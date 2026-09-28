#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "commandline.h"
#include "display.h"
#include "string.h"

void commandline_execute(char command[32]) {
  if (strcmp(command, "clear") == true) {
    terminal_init();
  }
  // make file
  // make folder
  // text editor
  // help
  //
  else {
    prints("Unkown command: ");
    prints(command);
    prints("\n");
  }
  prints(": ");
}
