#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "string.h"
#include "display.h"
#include "input.h"
#include "memory.h"

void clear(void) {
  terminal_init();
}

// in order of implementation :)
void help(void) {
  prints("clear - clears the screen\n");
  prints("memory - shows all available memory\n");
  prints("help - shows this help message\n\n");
}

void commandline_execute(char command[128]) {
  struct list cmd_list;
  string_sep(command, ' ', &cmd_list, 4, 32);
  if (strlen(command) == 0) {
    prints("\nPlease enter a command\n\n");
    return;
  }
  else if (strcmp(list_at(&cmd_list, 0), "clear")) {
    clear();
  }
  else if (strcmp(list_at(&cmd_list, 0), "memory")) {
    note("Press enter to read next memory segment, q to exit");
    display_mem(true);
    note("You are using bsOS");
  }
  else if (strcmp(list_at(&cmd_list, 0), "help")) {
    help();
  }
  else {
    prints("\nUnknown command: ");
    prints(command);
    prints("\n\n");
  }
  prints(": ");
}

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
