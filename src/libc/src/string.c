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
  if (strlen(str) != strlen(str2)) {
    return false;
  }
  for (size_t i = 0; i < strlen(str); i++) {
    if (str[i] != str2[i]) {
      return false;
    }
  }
  return true;
}

char* inttostring(const char*) {
  char* str;
  return str;
}
