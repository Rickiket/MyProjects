#include "s21_string.h"

char *s21_strstr(const char *haystack, const char *needle) {
  char *res = s21_NULL;
  s21_size_t i = 0, j = 0, first = 0;
  int fl = 0;
  if (*needle == '\0') res = (char *)haystack;
  while (haystack[i] != '\0' && fl == 0) {
    if (haystack[i] == needle[j]) {
      fl = 1;
      first = i;
      j = 0;
      while (haystack[i] != '\0' && needle[j] != '\0' && fl == 1) {
        if (haystack[i] != needle[j]) fl = 0;
        i++;
        j++;
      }
      if (fl == 1 && j == s21_strlen(needle)) res = (char *)&haystack[first];
    }
    i++;
  }
  return res;
}