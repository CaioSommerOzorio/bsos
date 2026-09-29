#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "string.h"
#include "display.h"

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;

// Define vga memory
static uint16_t* const terminal_buffer =
  (uint16_t*)VGA_MEMORY;

static uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
  return fg | bg << 4;
}

static uint16_t vga_entry(unsigned char c, uint8_t color) {
  return (uint16_t)c | (uint16_t)color << 8;
}

static inline void outb(uint16_t port, uint8_t value) {
  __asm__ volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

void scrolldown() {
  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t index = y * VGA_WIDTH + x;
      const size_t index2 = (y + 1) * VGA_WIDTH + x;
      terminal_buffer[index] = terminal_buffer[index2];
    }
  }
}

void update_cursor(uint8_t x, uint8_t y) {
  uint16_t pos = y * VGA_WIDTH + x;
  outb(0x3D4, 14);
  outb(0x3D5, pos >> 8);
  outb(0x3D4, 15);
  outb(0x3D5, pos);
}

void terminal_init(void) {
  terminal_row = 0;
  terminal_column = 0;

  terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t index = y * VGA_WIDTH + x;
      terminal_buffer[index] = vga_entry(' ', terminal_color);
    }
  }
}

void terminal_setcolor(uint8_t color) {
  terminal_color = color;
}

void print_uint64(uint64_t n) {
  char buf[20];
  int i = 0;

  if (n == 0) {
    putchar('0');
    return;
  }

  while (n > 0) {
    buf[i++] = '0' + n % 10;
    n /= 10;
  }

  while (i > 0) {
    putchar(buf[--i]);
  }
}

void print_sizet(size_t n) {
  char buf[11];
  int i = 0;

  if (n == 0) {
    putchar('0');
    return;
  }

  while (n > 0) {
    buf[i++] = '0' + n % 10;
    n /= 10;
  }

  while (i > 0) {
    putchar(buf[--i]);
  }
}

void putchar(char c) {
  if (c == '\n') {
    terminal_column = 0;
    if (terminal_row == VGA_HEIGHT - 1) {
      scrolldown();
    }
    else {
      terminal_row++;
    }
  }
  else if (c == '\b') {
    if (terminal_column > 0) {
      terminal_column--;
      putchar(' ');
      terminal_column--;
    }
    else {
      if (terminal_row > 0) {
        terminal_row--;
        terminal_column = VGA_WIDTH - 1;
      }
    }
  }
  else {
    const size_t index = terminal_row * VGA_WIDTH + terminal_column;
    terminal_buffer[index] = vga_entry(c, terminal_color);
    terminal_column++;

    if (terminal_column == VGA_WIDTH) {
      terminal_column = 0;
      terminal_row++;
    }
  }
  update_cursor(terminal_column, terminal_row);
}

void write(const char* data, size_t size) {
  for (size_t i = 0; i < size; i++)
    putchar(data[i]);
}

void prints(const char* data) {
  write(data, strlen(data));
}
