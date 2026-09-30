#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "string.h"
#include "display.h"
#include "input.h"

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

// press enter
void waitforinput (void) {
  while (1) {
    if (keyboard_get_scancode() == 0x1c) {
      break;
    }
  }
}
