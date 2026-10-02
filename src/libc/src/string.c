#include <stddef.h>
#include <stdbool.h>

#include "string.h"
#include "memory.h"

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
  for (size_t i = 0; i < size; i++) {
    if (string[i] == target) {
      string[i] = replacement;
    }
  }
}

void string_sep(char* string, char target, struct list *list, size_t size, size_t entry_size) {
  list_init(list, size, entry_size);
  size_t offset = 0;;
  char *buff = mem_hold(32);
  mem_set(buff, 0, 32);
  for (size_t i = 0; i <= strlen(string); i++) {
    if (string[i] == target || string[i] == '\0') {
      list_push(list, buff);
      mem_set(buff, 0, 32);
      offset = i+1;
    }
    else {
      buff[i-offset] = string[i];
    }
  }
}
