#include "s21_string.h"

void *s21_to_lower(const char *str) {
  s21_size_t n = s21_strlen(str);
  char *str2 = s21_NULL;
  if (str != s21_NULL) {
    str2 = malloc((n + 1) * sizeof(char));
    if (str2 != s21_NULL) {
      for (int i = 0; str[i] != '\0'; i++) {
        str2[i] = str[i];
        if (str[i] >= 65 && str[i] <= 90) str2[i] = str2[i] + 32;
      }
      str2[n] = '\0';
    }
  }
  return str2;
}