#include "s21_string.h"

void *s21_trim(const char *src, const char *trim_chars) {
  char *res = s21_NULL;
  const char def_ch[7] = " \t\n\v\r\f\0";
  if (src != s21_NULL) {
    const char *first = src;
    const char *end = src + s21_strlen(src) - 1;
    if (trim_chars == s21_NULL || s21_strlen(trim_chars) == 0) {
      while (*first && s21_strchr(def_ch, *first)) {
        first++;
      }
      while (end > first && s21_strchr(def_ch, *end)) {
        end--;
      }
    } else {
      while (*first && s21_strchr(trim_chars, *first)) {
        first++;
      }
      while (end > first && s21_strchr(trim_chars, *end)) {
        end--;
      }
    }
    s21_size_t len = end - first + 1;
    res = malloc((len + 1) * sizeof(char));
    if (res != s21_NULL) {
      s21_strncpy(res, first, len);
      res[len] = '\0';
    }
  }
  return res;
}