#include "s21_string.h"

void* s21_memchr(const void* str, int c, s21_size_t n) {
  char* result = s21_NULL;
  if (str != NULL) {
    s21_size_t i = 0;

    for (; i < n; i++) {
      if (*((char*)str + i) == c) {
        result = (char*)str + i;
        i = n;
      }
    }
  }

  return result;
}
