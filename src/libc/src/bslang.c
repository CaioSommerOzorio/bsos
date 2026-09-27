#include <stddef.h>
#include <stdbool.h>

#include "bslang.h"
#include "display.h"
#include "string.h"

void bslang(char command[32]) {
  if (strcmp(command, "clear") == true) {
    terminal_initialize();
  }
}

void testfunc(void) {
  
}
