#ifndef DISPLAY_H
#define DISPLAY_H

#include <stddef.h>
#include <stdint.h>

#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

// Clears everything on the screen (sets it to ' ')
// Sets colour to white on black
void terminal_init(void);

// Literally just declares a variable
void terminal_setcolor(uint8_t color);

// Prints a single character, wraps if necessary
// Can handle newline and backspace
void putchar(char c);

// Prints a string by iterating and calling putchar
void write(const char* data, size_t size);

// Same as write but can figure out the size by itself
void prints(const char* data);
void print_sizet(size_t n);
void print_uint64(uint64_t n);

// Sets the text on bar at the bottom of the screen
// Gets cleared by terminal_init
// Doesn't get overwritten by normal text, doesn't scroll down like normal text
void note(const char* string);

#endif
