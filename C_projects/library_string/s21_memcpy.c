#include "s21_string.h"

void* s21_memcpy(void* dest, const void* src, s21_size_t n) {
  if (src != s21_NULL && dest != s21_NULL) {
    s21_size_t i = 0;
    unsigned char* ptr_dest = (unsigned char*)dest;
    const unsigned char* ptr_src = (const unsigned char*)src;
    for (; i < n; i++) {
      ptr_dest[i] = ptr_src[i];
    }
  }
  return dest;
}