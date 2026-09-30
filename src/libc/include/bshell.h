#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

void commandline_execute(char command[32]);
void waitforinput(void);

#endif
