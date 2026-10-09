#include <stdio.h>
#include <stdlib.h>
typedef unsigned long long s21_size_t;
#define s21_NULL (void *)0

s21_size_t s21_strcspn(const char *str1, const char *str2);
char *s21_strtok(char *str, const char *delim);
void find_end(const char *delim, int *fl, const char *tmp, char *str,
              const s21_size_t i);
char *s21_strerror(int errnum);

void *s21_memchr(const void *str, int c, s21_size_t n);
int s21_memcmp(const void *lhs, const void *rhs, s21_size_t count);
void *s21_memcpy(void *dest, const void *src, s21_size_t n);
void *s21_memset(void *str, int c, s21_size_t n);
char *s21_strchr(const char *str, int c);

s21_size_t s21_strlen(const char *str);
char *s21_strncat(char *dest, const char *src, s21_size_t n);
int s21_strncmp(const char *str1, const char *str2, s21_size_t n);
char *s21_strncpy(char *dest, const char *src, s21_size_t n);
char *s21_strpbrk(const char *str1, const char *str2);

char *s21_strrchr(const char *str, int symbol);
char *s21_strstr(const char *haystack, const char *needle);
void *s21_to_lower(const char *str);
void *s21_to_upper(const char *str);

void *s21_insert(const char *src, const char *str, s21_size_t start_index);
void *s21_trim(const char *src, const char *trim_chars);
