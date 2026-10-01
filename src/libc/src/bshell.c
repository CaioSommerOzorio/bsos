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
  prints("help - shows this help message\n");
}

void commandline_execute(char command[32]) {
  if (strlen(command) == 0) {
    prints("Please enter a command\n\n: ");
    return;
  }
  char *buff = before_character(command, ' ', 32);
  if (strcmp(buff, "clear")) {
    clear();
  }
  else if (strcmp(buff, "memory")) {
    note("Press enter to read next memory segment, q to exit");
    display_mem(true);
    note("You are using bsOS");
  }
  else if (strcmp(buff, "help")) {
    help();
    buff = after_character(command, ' ', 32);
    prints(buff);
  }
  else {
    prints("Unknown command: ");
    prints(command);
    prints("\n");
  }
  prints("\n: ");
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
