#include <stdint.h>
#include <stddef.h>
#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_BUFFER ((volatile uint16_t*)0xB8000)

static size_t cursor_x = 0;
static size_t cursor_y = 0;
static uint8_t current_color = 0x0F;

char scancode_to_ascii[] = { 0,0,'1','2','3','4','5','6','7','8','9','0','*','-',0,0,'q','w','e','r','t','y','u','i','o','p','\xF0','\xFC',0,0,'a','s','d','f','g','h','j','k','l','\xFE','i','"',0,',','z','x','c','v','b','n','m','\xF6','\xE7','.',0,'*',0,' ',0,0,0,0,0,0,0,0,0,0,0,0,'7','8','9','-','4','5','6','+','1','2','3','0','.',0,0,0,0,0 };
char scancode_to_ascii_shift[] = { 0,0,'!','\'','#','+','%','&','/','\xE6','(',')','?','_',0,0,'Q','W','E','R','T','Y','U','I','O','P','\xD0','\xDC',0,0,'A','S','D','F','G','H','J','K','L','\xDE','\xC4','<',0,'>','Z','X','C','V','B','N','M','\xD6','\xC7',':',0,'*',0,' ',0,0,0,0,0,0,0,0,0,0,0,0,'7','8','9','-','4','5','6','+','1','2','3','0','.',0,0,0,0,0 };
static int shift_pressed = 0;

static inline uint8_t inb(uint16_t port) {
    uint8_t val;
    __asm__ volatile ("inb %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

uint8_t keyboard_read() {
    while (!(inb(0x64) & 0x1));
    return inb(0x60);
}

uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | ((uint16_t)color << 8);
}

static void vga_putc_at(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    VGA_BUFFER[index] = vga_entry(c, color);
}

static void vga_putc(char c) {
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
    if (shift_pressed)
        return scancode_to_ascii_shift[sc];
    else
        return scancode_to_ascii[sc];
}

void kernel_main(void) {
    print_at("Welcome to denemOS!", 0x87, 0, 24);
    print_at("v0.1", 0x87, 76, 24);
    print_at("                                                         ", 0x87, 19, 24);
    char yazi[256];
    int len = 0;
    while(1) {
        print("> ");
        while(1) {
            char c = get_char();
            if (c == 0) continue;
            if (c == '\n') {
                yazi[len] = '\0';
                len = 0;
                print("\nNo command found\n");
                break;
            }
            yazi[len++] = c;
            vga_putc(c);
            if (len >= 255) break;
        }
    }
}
