#include "s21_string.h"
char *s21_strrchr(const char *str, int symbol) {
  int strlength = s21_strlen(str);
  const char *res = s21_NULL;

  for (int x = strlength; res == s21_NULL && x >= 0; x -= 1) {
    if (str[x] == symbol) res = (str + x);
  }

  return (char *)res;
}