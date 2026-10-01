#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <stdbool.h>

size_t strlen(const char* str);
bool strcmp(const char* str, const char* str2);
void string_replace(char* string, char target, char replacement, size_t size);
char *before_character(char* string, char c, size_t size);

#endif
