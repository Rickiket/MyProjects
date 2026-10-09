#include "s21_string.h"

char *s21_strpbrk(const char *str1, const char *str2) {
  char *res = s21_NULL;
  s21_size_t i = 0, j = 0;
  while (str1[i] != '\0' && res == s21_NULL) {
    while (str2[j] != '\0') {
      if (str1[i] == str2[j]) res = (char *)&str1[i];
      j++;
    }
    j = 0;
    i++;
  }
  return res;
}