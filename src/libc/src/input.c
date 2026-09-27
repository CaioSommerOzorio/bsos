#include <stdint.h>

static inline uint8_t inb(uint16_t port) {
  uint8_t result;
  asm volatile ("inb %1, %0" : "=a" (result) : "dN" (port));
  return result;
}

uint8_t keyboard_get_scancode(void) {
  while (!(inb(0x64) & 1));

  return inb(0x60);
}

char scancode_to_ascii(uint8_t scancode) {
  switch (scancode) {
    case 0x1E: return 'a';
    case 0x30: return 'b';
    case 0x2E: return 'c';
    case 0x20: return 'd';
    case 0x12: return 'e';
    case 0x21: return 'f';
    case 0x22: return 'g';
    case 0x23: return 'h';
    case 0x17: return 'i';
    case 0x24: return 'j';
    case 0x25: return 'k';
    case 0x26: return 'l';
    case 0x32: return 'm';
    case 0x31: return 'n';
    case 0x18: return 'o';
    case 0x19: return 'p';
    case 0x10: return 'q';
    case 0x13: return 'r';
    case 0x1F: return 's';
    case 0x14: return 't';
    case 0x16: return 'u';
    case 0x2F: return 'v';
    case 0x11: return 'w';
    case 0x2D: return 'x';
    case 0x15: return 'y';
    case 0x2C: return 'z';
    case 0x39: return '0';
    case 0x3A: return '1';
    case 0x3B: return '2';
    case 0x3C: return '3';
    case 0x3D: return '4';
    case 0x3E: return '5';
    case 0x3F: return '6';
    case 0x40: return '7';
    case 0x41: return '8';
    case 0x42: return '9';
    case 0x2A: return ' ';
    case 0x1C: return '\n';
    case 0x45: return '\b';
    default: return 0;
  }
}

char getchar(void) {
  while (1) {
    uint8_t scancode = keyboard_get_scancode();
    if (scancode & 0x80) {
      continue;
    }

    char c = scancode_to_ascii(scancode);

    if (c) {
      return c;
    }
  }
}
