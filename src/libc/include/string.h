#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <stdbool.h>

#include <memory.h>

size_t strlen(const char* str);
bool strcmp(const char* str, const char* str2);
void string_replace(char* string, char target, char replacement, size_t size);
void string_sep(char* string, char target, struct list *list, size_t size, size_t entry_size);

#endif
