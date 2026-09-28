#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "commandline.h"
#include "display.h"
#include "string.h"

void commandline_execute(char command[32]) {
  if (strcmp(command, "clear") == true) {
    terminal_initialize();
  }
  // make file
  // make folder
  // text editor
  // help
  //
  else {
    terminal_writestring("Unkown command: ");
    terminal_writestring(command);
    terminal_writestring("\n");
  }
  terminal_writestring(": ");
}
