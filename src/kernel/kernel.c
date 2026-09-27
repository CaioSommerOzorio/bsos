#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Custom lib files
#include <display.h>
#include <string.h>
#include <input.h>
#include <bslang.h>

#if defined(__linx__)
#error "just use a cross compiler bro"
#endif

#if !defined(__i386__)
#error "wrong conpiler bro use ix86-elf"
#endif

// Kernel entry point
void kernel_main(void) {
  terminal_initialize();

  terminal_writestring("Hello, world!\n");
  char command[32];
  char input;

  while (1) {
    input = getchar();
    if (!(input == '\n' || input == '\b')) {
      if (strlen(command) < 32) {
        command[strlen(command)] = input;
      }
    }
    terminal_putchar(input);
    if (input == '\n') {
      testfunc();
      bslang(command);
      memset(&command, 0, 32);
    }
  }
}
