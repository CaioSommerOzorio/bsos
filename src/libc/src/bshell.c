#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "string.h"
#include "display.h"
#include "input.h"
#include "memory.h"

// Enter returns 0
// q returns 1
size_t waitforinput (void) {
  while (1) {
    char a = getchar();
    if (a == '\n') {
      return 0;
    }
    else if (a == 'q') {
      return 1;
    }
  }
}

void clear(void) {
  terminal_init();
}

void make_file(char* name) {
  void *file_addr = mem_hold(32);
  struct file newfile;
  mem_copy(name, newfile.name, 32);
  newfile.addr = file_addr;
  list_push(fs.files, &newfile);
  prints("\nFile created\n\n");
}

// genuinely have no idea why i have to do this lol but whatever works
void display_files(struct list *list) {
  prints("\nNumber of files: ");
  print_sizet(list->item_count);
  prints("\n");
  for (size_t i = 0; i < list->item_count; i++) {
    prints("\n");
    prints((char*)list->addr + i * list->entry_size);
  }
  prints("\n\n");
}

void show_files() {
  struct list *files = (struct list *)fs.files;
  display_files((struct list *)fs.files);
}

// for whatever reason THIS DOES NOT WORK
// not sure why but it just reboots it in qemu
//void show_files() {
//  struct list *list = (struct list *)fs.files;
//  prints("\nNumber of files: ");
//  print_sizet(list->item_count);
//  prints("\n\n");
//  for (size_t i = 0; i < list->item_count; i++) {
//    prints((char*)list->addr + i * list->entry_size);
//    prints("\n");
//  }
//}

// in order of implementation :)
void help(void) {
  prints("\nclear - clears the screen\n");
  prints("memory - shows all available memory\n");
  prints("help - shows this help message\n");
  prints("mkf fileName - creates a file\n");
  prints("files - lists all files\n");
  prints("\n");
}

void commandline_execute(char command[128]) {
  struct list cmd_list;
  string_sep(command, ' ', &cmd_list, 4, 32);
  char *cmd = list_at(&cmd_list, 0);
  char *arg1 = list_at(&cmd_list, 1);
  if (strlen(command) == 0) {
    prints("\nPlease enter a command\n\n");
    return;
  }
  else if (strcmp(cmd, "clear")) {
    clear();
  }
  else if (strcmp(cmd, "memory")) {
    note("Press enter to read next memory segment, q to exit");
    display_mem(true);
    note("You are using bsOS");
  }
  else if (strcmp(cmd, "help")) {
    help();
  }
  else if (strcmp(cmd, "mkf")) {
    if (strlen(arg1) == 0) {
      prints("\nPlease enter a file name\n\n");
    }
    else if (strlen(arg1) > 32) {
      prints("\nFile name must be less than 32 characters\n\n");
    }
    else {
      make_file(arg1);
    }
  }
  else if (strcmp(cmd, "files")) {
    show_files();
  }
  else {
    prints("\nUnknown command: ");
    prints(command);
    prints("\n\n");
  }
  prints(": ");
}
