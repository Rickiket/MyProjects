#include "s21_string.h"

int s21_memcmp(const void* lhs, const void* rhs, s21_size_t count) {
  s21_size_t i = 0;
  int result = 0;
  unsigned char* p1 = (unsigned char*)lhs;
  unsigned char* p2 = (unsigned char*)rhs;
  for (; i < count; i++) {
    if (p1[i] != p2[i]) {
      result = (int)p1[i] - (int)p2[i];
      i = count;
    }
  }
  return result;
}