#include "s21_string.h"
int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  int res = 0;

  while (n != 0 && *str1 != '\0' && *str2 != '\0') {
    if (*str1 != *str2) {
      res = (*(unsigned char *)str1 - *(unsigned char *)str2);
      n = 0;
    } else {
      str1++;
      str2++;
      n--;
    }
  }

  if (n != 0) {
    res = (*(unsigned char *)str1 - *(unsigned char *)str2);
  }

  return res;
}