#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>

static inline uint8_t inb(uint16_t port);
uint8_t keyboard_get_scancode(void);
char scancode_to_ascii(uint8_t scancode);
char getchar(void);

#endif
