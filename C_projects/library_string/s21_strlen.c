#include "s21_string.h"
s21_size_t s21_strlen(const char *str) {
  s21_size_t strlength;
  for (strlength = 0; str[strlength] != '\0'; strlength++);
  return strlength;
}