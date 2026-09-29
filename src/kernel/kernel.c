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
  terminal_init();

  struct mb_tag *tag =
    (struct mb_tag*)((char *)mb_info + 8);

  while (tag->type != 0) {
    if (tag->type == 6) {
      struct mmap_entry *mmap =
        (struct mmap_entry *)((char *)tag+16);
      while ((char*)mmap < (char *)tag + tag->size) {
        prints("Address: ");
        print_uint64(mmap->addr);
        prints("\nLength: ");
        print_uint64(mmap->len);
        prints("\nType: ");
        print_uint64(mmap->type);
        prints("\n\n");
        mmap =
          (struct mmap_entry *)((char *)mmap + tag->entry_size);
      }
    }

    tag = (struct mb_tag *)
      ((char *)tag + ((tag->size + 7) & ~7));
  }

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
