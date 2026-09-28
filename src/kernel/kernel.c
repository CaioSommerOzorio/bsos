#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Custom lib files
#include <display.h>
#include <string.h>
#include <input.h>
#include <commandline.h>

#if defined(__linx__)
#error "just use a cross compiler bro"
#endif

#if !defined(__i386__)
#error "wrong conpiler bro use ix86-elf"
#endif

// Kernel entry point
void kernel_main(void *multiboot_info) {
  terminal_initialize();

  terminal_writestring("\nWelcome to bsOS!\n");
  terminal_writestring(": ");
  char command[32];
  char input;

  while (1) {
    input = getchar();
    if (input != '\n') {
      if (strlen(command) < 32) {
        command[strlen(command)] = input;
        terminal_putchar(input);
      }
    }
    if (input == '\n') {
      terminal_putchar(input);
      commandline_execute(command);
      memset(&command, 0, 32);
    }
  }
}
