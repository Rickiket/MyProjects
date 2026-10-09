#include "s21_string.h"

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  s21_size_t i = 0;
  if (str1 != s21_NULL && str2 != s21_NULL) {
    s21_size_t j = 0;
    int f = 0;
    while ((str1[i] != str2[j]) && f == 0 && str1[i] != '\0') {
      while (str2[j] != '\0') {
        if (str1[i] == str2[j]) f = 1;
        j++;
      }
      j = 0;
      if (f != 1) i++;
    }
  }
  return i;
}