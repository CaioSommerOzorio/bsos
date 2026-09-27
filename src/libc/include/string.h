#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <stdbool.h>

size_t strlen(const char* str);
bool strcmp(const char* str, const char* str2);
void memset(void* ptr, int value, size_t num);

#endif
