#include "kernel.h"

static size_t cursor_x = 0;
static size_t cursor_y = 0;
static uint8_t current_color = COLOR_DEFAULT;

char scancode_to_ascii[] = { 0,0,'1','2','3','4','5','6','7','8','9','0','*','-',0,0,'q','w','e','r','t','y','u','i','o','p','\xF0','\xFC',0,0,'a','s','d','f','g','h','j','k','l','\xFE','i','"',0,',','z','x','c','v','b','n','m','\xF6','\xE7','.',0,'*',0,' ',0,0,0,0,0,0,0,0,0,0,0,0,'7','8','9','-','4','5','6','+','1','2','3','0','.',0,0,0,0,0 };
char scancode_to_ascii_shift[] = { 0,0,'!','\'','#','+','%','&','/','\xE6','(',')','?','_',0,0,'Q','W','E','R','T','Y','U','I','O','P','\xD0','\xDC',0,0,'A','S','D','F','G','H','J','K','L','\xDE','\xC4','<',0,'>','Z','X','C','V','B','N','M','\xD6','\xC7',':',0,'*',0,' ',0,0,0,0,0,0,0,0,0,0,0,0,'7','8','9','-','4','5','6','+','1','2','3','0','.',0,0,0,0,0 };
static int shift_pressed = 0;

static inline uint8_t inb(uint16_t port) {
    uint8_t val;
    __asm__ volatile ("inb %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

uint8_t keyboard_read() {
    while (!(inb(0x64) & 0x1)); // Veri gelene kadar bekle
    return inb(0x60);
}

uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | ((uint16_t)color << 8);
}

static void vga_putc_at(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    VGA_BUFFER[index] = vga_entry(c, color);
}

void vga_putc(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
        if (cursor_y >= VGA_HEIGHT) cursor_y = VGA_HEIGHT - 1;
        return;
    }
    if (c == '\b') {
        if (cursor_x > 0) {
            cursor_x--;
            vga_putc_at(' ', current_color, cursor_x, cursor_y);
        }
        return;
    }
    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
        if (cursor_y >= VGA_HEIGHT) cursor_y = VGA_HEIGHT - 1;
    }
    vga_putc_at(c, current_color, cursor_x, cursor_y);
    cursor_x++;
}

void print(const char *str) {
    for (const char *p = str; *p; ++p)
        vga_putc(*p);
}

void print_at(const char *str, uint8_t color, size_t x, size_t y) {
    for (const char *p = str; *p; ++p) {
        vga_putc_at(*p, color, x, y);
        x++;
        if (x >= VGA_WIDTH) { x = 0; y++; }
    }
}

char get_char() {
    uint8_t sc = keyboard_read();
    if (sc == 0x0E) return '\b';
    if (sc == 0x2A || sc == 0x36) { shift_pressed = 1; return 0; }
    if (sc == 0xAA || sc == 0xB6) { shift_pressed = 0; return 0; }
    if (sc & 0x80) return 0;
    if (sc == 0x1C) return '\n';
    return (shift_pressed) ? scancode_to_ascii_shift[sc] : scancode_to_ascii[sc];
}

void run_cmd(disk *dd, char *cmd) {
    if (cmd[0] == 'l' && cmd[1] == 's') {
        ls(dd);
    } else {
        print("bilinmeyen komut: ");
        print(cmd);
        print("\n");
    }
}

void kernel_main() {
    disk *dd = dattach(DRIVE_MASTER);
    fs_mount(dd);

    fs_create(dd, (int8 *)"test.txt");
    fs_write(dd, (int8 *)"test.txt", "merhaba dunya", 13);

    print("denemOS v0.2\n");
    print("> ");

    char cmd[64];
    int  cmd_len = 0;

    while (1) {
        char c = get_char();
        if (c == 0) continue;

        if (c == '\n') {
            cmd[cmd_len] = '\0';
            print("\n");

            /* komutları buraya ekleyeceğiz */
            if (cmd_len > 0) {
                // komut çalıştır
                run_cmd(dd, cmd);
            }

            cmd_len = 0;
            print("> ");
        } else if (c == '\b') {
            if (cmd_len > 0) {
                cmd_len--;
                vga_putc('\b');
            }
        } else {
            if (cmd_len < 63) {
                cmd[cmd_len++] = c;
                vga_putc(c);
            }
        }
    }
}