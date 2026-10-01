#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

void commandline_execute(char command[32]);
size_t waitforinput(void);

#endif
