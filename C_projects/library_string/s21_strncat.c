#include "s21_string.h"
char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  if (dest == s21_NULL || src == s21_NULL) {
    return s21_NULL;
  }

  char *original_dest = dest;
  while (*dest != '\0') {
    dest++;
  }

  while (*src != '\0' && n > 0) {
    *dest++ = *src++;
    n--;
  }
  *dest = '\0';

  return original_dest;
}