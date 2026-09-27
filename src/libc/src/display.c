#include <stddef.h>
#include <stdint.h>

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

void terminal_initialize(void) {
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

void terminal_putchar(char c) {
  if (c == '\n') {
    terminal_column = 0;
    terminal_row++;
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
}

void terminal_write(const char* data, size_t size) {
  for (size_t i = 0; i < size; i++)
    terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) {
  terminal_write(data, strlen(data));
}
