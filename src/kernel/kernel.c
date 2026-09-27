#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <display.h>
#include <string.h>

#if defined(__linx__)
#error "just use a cross compiler bro"
#endif

#if !defined(__i386__)
#error "wrong conpiler bro use ix86-elf"
#endif

void kernel_main(void) {
  terminal_initialize();

  terminal_writestring("Hello, world!\n");
}
