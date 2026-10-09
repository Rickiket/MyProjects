#include "s21_string.h"

char *s21_strtok(char *str, const char *delim) {
  static char *token;
  if (str == s21_NULL) str = token;

  while (*str != '\0' && s21_strcspn(str, delim) == 0) str++;
  char *tmp = str;
  if (tmp[0] != '\0') {
    int fl = 0;
    s21_size_t i = 0;
    while (*tmp != '\0' && fl == 0) {
      find_end(delim, &fl, tmp, str, i);
      i++;
      tmp++;
    }
  } else {
    str = s21_NULL;
  }
  token = tmp;
  return str;
}

void find_end(const char *delim, int *fl, const char *tmp, char *str,
              const s21_size_t i) {
  s21_size_t j = 0;
  while (delim[j] != '\0' && *fl == 0) {
    if (delim[j] == *tmp) {
      *fl = 1;
      str[i] = '\0';
    }
    j++;
  }
}