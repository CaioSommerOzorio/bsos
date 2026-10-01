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

  terminal_init();

  prints("Memory map\n====================================\n");
  prints("Number of usable memory segments: ");
  print_sizet(mm_count);
  prints("\n\n");
  for (size_t i = 0; i < mm_count; i++) {
    prints("Memory number ");
    print_sizet(i);
    prints(": \nAddress: ");
    print_uint64(mem_sectors[i].addr);
    prints("\nLength: ");
    print_uint64(mem_sectors[i].len);
    prints("\n\n");
  }

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
    else {
      putchar(input);
      commandline_execute(command);
      mem_set(&command, 0, 32);
      command_len = 0;
    }
  }
}
