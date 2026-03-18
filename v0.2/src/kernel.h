/* kernel.h */
#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <stddef.h>
#include "disk.h"
#include "fs.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_BUFFER ((volatile uint16_t*)0xB8000)

#define COLOR_DEFAULT 0x0F
#define COLOR_STATUS_BAR 0x87

#define packed __attribute__((packed))
#define internal static

typedef int8_t  int8;
typedef int16_t int16;
typedef int32_t int32;

void kernel_main(void);
void print(const char *str);
void print_at(const char *str, uint8_t color, size_t x, size_t y);
void vga_putc(char c);
char get_char();
uint8_t keyboard_read();

#endif