#include <stddef.h>
#include <stdbool.h>

#include "string.h"

size_t strlen(const char* str) {
  size_t len = 0;
  for (size_t i = 0; str[i] != '\0'; i++) {
    len++;
  }
  return len;
}

bool strcmp(const char* str, const char* str2) {
  size_t smallest_string = strlen(str) < strlen(str2) ? strlen(str) : strlen(str2);
  for (size_t i = 0; i < smallest_string; i++) {
    if (str[i] != str2[i]) {
      return false;
    }
  }
  return true;
}

void memset(void* ptr, int value, size_t size) {
  for (size_t i = 0; i < size; i++) {
    ((char*)ptr)[i] = value;
  }
}
