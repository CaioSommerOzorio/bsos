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

struct multiboot_tag {
  size_t tag;
  size_t size;
};

// Multiboot info gets passed in
void kernel_main(void* mb_info) {
  terminal_init();

  struct multiboot_tag* first_tag = (struct multiboot_tag*)((char* )mb_info + 8);
  prints((char*)first_tag->tag);

  prints("\nWelcome to bsOS!\n");
  prints(": ");
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
      memset(&command, 0, 32);
      command_len = 0;
    }
  }
}
