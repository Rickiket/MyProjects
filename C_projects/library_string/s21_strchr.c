#include "s21_string.h"
char* s21_strchr(const char* str, int c) {
  char* result = s21_NULL;
  for (int i = 0; *(str + i) != '\0'; i++) {
    if (*(str + i) == (char)c) {
      result = (char*)(str + i);
    }
  }
  return result;
}
