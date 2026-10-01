#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Custom lib files
#include <display.h>
#include <string.h>
#include <input.h>
#include <bshell.h>
#include <memory.h>

#if defined(__linx__)
#error "just use a cross compiler bro"
#endif

#if !defined(__i386__)
#error "wrong conpiler bro use ix86-elf"
#endif

void kernel_main(void* mb_info) {
  struct mmap_entry mem_sectors[32];
  size_t mm_count = parse_mmap(mb_info, mem_sectors, 32);
  mem_init(mem_sectors, mm_count);

  terminal_init();

  note("You are using bsOS");
  display_mem(false);
  prints("Welcome to bsOS!");

  prints("\n: ");

  char command[32];
  size_t command_len = 0;
  char input;

  while (1) {
    input = getchar();
    if (input != '\n') {
      // can't be buffer overflow and can't backspace if there is no input yet
      if (strlen(command) < 32 && !(strlen(command) == 0 && input == '\b')) {
        if (input != '\b') {
          command[command_len] = input;
          command_len++;
        }
        else {
          command[command_len-1] = '\0';
          command_len--;
        }
        putchar(input);
      }
    }
    // enter runs command
    else {
      putchar('\n');
      commandline_execute(command);
      mem_set(&command, 0, 32);
      command_len = 0;
    }
  }
}
