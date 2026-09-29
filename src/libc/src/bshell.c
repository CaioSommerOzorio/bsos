#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "string.h"
#include "display.h"

void commandline_execute(char command[32]) {
  if (strcmp(command, "clear")) {
    terminal_init();
  }
  else {
    prints("Unknown command: ");
    prints(command);
    prints("\n");
  }
  prints(": ");
}

