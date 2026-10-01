#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>

// inb is the assembly instruction to read a byte from io port
// returns port data
uint8_t inb(uint16_t port);

// returns scancode
uint8_t keyboard_get_scancode(void);

// converts scancode to ascii
char scancode_to_ascii(uint8_t scancode);

// waits for a key to be pressed then returns it
char getchar(void);

#endif
