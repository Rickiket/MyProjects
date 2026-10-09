#include "s21_string.h"

void* s21_memset(void* str, int c, s21_size_t n) {
  if (str != NULL) {
    s21_size_t i = 0;
    for (; i < n; i++) {
      *((char*)str + i) = (char)c;
    }
  }
  return str;
}