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

char *inttostring(const char*) {
  char* str;
  return str;
}

void string_replace(char* string, char target, char replacement, size_t size) {
  size_t i = 0;
  for (size_t i = 0; i < size; i++) {
    if (string[i] == target) {
      string[i] = replacement;
    }
  }
}


// returns everything before the character
char *before_character(char *string, char c, size_t size) {
  static char buff[32];
  size_t i;

  for (i = 0; i < size && string[i] != c && string[i] != '\0'; i++) {
    buff[i] = string[i];
  }
  buff[i] = '\0';

  return buff;
}
