

#include <check.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../s21_string.h"

START_TEST(strlen1) {
  const char string[10] = "world";
  ck_assert_int_eq(strlen(string), s21_strlen(string));
}
END_TEST

START_TEST(strlen2) {
  char string1[100] = "hello world";
  ck_assert_int_eq(strlen(string1), s21_strlen(string1));
}
END_TEST

START_TEST(memchr1) {
  const char *string = "world";
  ck_assert_ptr_eq(s21_memchr(string, 'w', 5), memchr(string, 'w', 5));
}
END_TEST

START_TEST(memchr2) {
  const char *string = "";
  ck_assert_ptr_eq(s21_memchr(string, 'w', 1), memchr(string, 'w', 1));
}
END_TEST

START_TEST(memcmp1) {
  const char *string = "asd";
  const char *string2 = "asddasd";
  ck_assert_int_eq(s21_memcmp(string, string2, 4), memcmp(string, string2, 4));
}
END_TEST

START_TEST(memcmp2) {
  const char *string = "asd";
  const char *string2 = "asd";
  ck_assert_int_eq(s21_memcmp(string, string2, 4), memcmp(string, string2, 4));
}
END_TEST

START_TEST(memcpy1) {
  char source[] = "Hello World";
  char destination[5];
  ck_assert_str_eq(s21_memcpy(destination, source, 5),
                   memcpy(destination, source, 5));
}
END_TEST

START_TEST(memcpy2) {
  char source[] = "#^$%#";
  char destination[5];
  ck_assert_str_eq(s21_memcpy(destination, source, 5),
                   memcpy(destination, source, 5));
}
END_TEST

START_TEST(memset1) {
  char source[] = "1234";
  char source2[] = "1234";
  s21_memset(source, '9', 2);
  memset(source2, '9', 2);
  ck_assert_str_eq(source, source2);
}
END_TEST

START_TEST(memset2) {
  char source[] = "1234";
  char source2[] = "1234";
  s21_memset(source, '(', 4);
  memset(source2, '(', 4);
  ck_assert_str_eq(source, source2);
}
END_TEST

START_TEST(strncat1) {
  char string2[100] = "hello ";
  char string3[100] = "world";
  ck_assert_str_eq(strncat(string2, string3, 5),
                   s21_strncat(string2, string3, 5));
}
END_TEST

START_TEST(strncat2) {
  char string2[100] = "      ";
  char string3[100] = "world";
  ck_assert_str_eq(strncat(string2, string3, 5),
                   s21_strncat(string2, string3, 5));
}
END_TEST

START_TEST(strncat3) {
  char string2[100] = "      ";
  char *dest = s21_NULL;
  char *result = s21_strncat(dest, string2, 5);
  ck_assert_ptr_eq(result, s21_NULL);
}
END_TEST

START_TEST(strncmp1) {
  char string4[100] = "hello world";
  char string5[100] = "hello world";
  ck_assert_int_eq(strncmp(string4, string5, 11),
                   s21_strncmp(string4, string5, 11));
}
END_TEST

START_TEST(strncmp2) {
  char string4[100] = "gello world";
  char string5[100] = "hello world";
  ck_assert_int_eq(strncmp(string4, string5, 11),
                   s21_strncmp(string4, string5, 11));
}
END_TEST

START_TEST(strncmp3) {
  char string4[100] = "hello";
  char string5[100] = "hello world";
  ck_assert_int_eq(strncmp(string4, string5, 10) < 0,
                   s21_strncmp(string4, string5, 10) < 0);
}
END_TEST

START_TEST(strncpy1) {
  char string6[100] = "                     ";
  char string7[100] = "hello world";
  ck_assert_str_eq(strncpy(string6, string7, 5),
                   s21_strncpy(string6, string7, 5));
}
END_TEST

START_TEST(strncpy2) {
  char *src = "Hello, World!";
  char *dest = s21_NULL;
  char *result = s21_strncpy(dest, src, 5);
  ck_assert_ptr_eq(result, s21_NULL);
}
END_TEST

START_TEST(strncpy3) {
  char string6[100] = "                     ";
  char string7[100] = "hello world";
  ck_assert_str_eq(strncpy(string6, string7, 20),
                   s21_strncpy(string6, string7, 20));
}
END_TEST

START_TEST(strrchr1) {
  char string8[100] = "hello world";
  ck_assert_str_eq(strrchr(string8, 'o'), s21_strrchr(string8, 'o'));
}
END_TEST

START_TEST(strrchr2) {
  char string8[100] = "school";
  ck_assert_str_eq(strrchr(string8, 'l'), s21_strrchr(string8, 'l'));
}
END_TEST

START_TEST(strrchr3) {
  char str[] = "12345678910";
  char *result = s21_strrchr(str, '4');
  char *expected = strrchr(str, '4');
  ck_assert_ptr_eq(result, expected);
}
END_TEST

START_TEST(strrchr4) {
  char str[] = "12345678910";
  char *result = s21_strrchr(str, 'b');
  char *expected = strrchr(str, 'b');
  ck_assert_ptr_eq(result, expected);
}
END_TEST

START_TEST(strerror1) {
  for (int i = 0; i < 106; i++) {
    const char *std_err = strerror(i);
    const char *s21_err = s21_strerror(i);
    ck_assert_str_eq(std_err, s21_err);
  }
}
END_TEST

START_TEST(strcspn1) {
  char str1[] = "adekk";
  char str2[] = "adef";
  s21_size_t result = s21_strcspn(str1, str2);
  s21_size_t expected = strcspn(str1, str2);
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(strcspn2) {
  char str1[] = "adef";
  char str2[] = "adef";
  s21_size_t result = s21_strcspn(str1, str2);
  s21_size_t expected = strcspn(str1, str2);
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(strcspn3) {
  char str1[] = "f";
  char str2[] = "asda";
  s21_size_t res = s21_strcspn(str1, str2);
  s21_size_t expected = strcspn(str1, str2);
  ck_assert_int_eq(res, expected);
}
END_TEST

START_TEST(strcspn4) {
  char str1[] = "";
  char str2[] = "";
  s21_size_t res = s21_strcspn(str1, str2);
  s21_size_t expected = strcspn(str1, str2);
  ck_assert_int_eq(res, expected);
}
END_TEST

START_TEST(strpbrk1) {
  char str1[10] = "123456789";
  char str2[10] = "123";
  char *result = s21_strpbrk(str1, str2);
  char *expected = strpbrk(str1, str2);
  ck_assert_str_eq(result, expected);
}
END_TEST

START_TEST(strpbrk2) {
  char str1[10] = "";
  char str2[10] = "123";
  char *result = s21_strpbrk(str1, str2);
  char *expected = strpbrk(str1, str2);
  if (expected == NULL)
    ck_assert_ptr_null(result);
  else
    ck_assert_str_eq(result, expected);
}
END_TEST

START_TEST(strstr1) {
  char str1[10] = "123456789";
  char str2[10] = "678";
  char *result = s21_strstr(str1, str2);
  char *expected = strstr(str1, str2);
  ck_assert_ptr_eq(result, expected);
}
END_TEST

START_TEST(strstr2) {
  char str1[10] = "";
  char str2[10] = "678";
  char *result = s21_strstr(str1, str2);
  char *expected = strstr(str1, str2);
  ck_assert_ptr_eq(result, expected);
}
END_TEST

START_TEST(strtok1) {
  char str[] = "   ";
  char *delim = " ";
  char *s21_token = s21_strtok(str, delim);
  char *std_token = strtok(str, delim);
  ck_assert_ptr_eq(s21_token, std_token);
}
END_TEST

START_TEST(strtok2) {
  char str1[] = "";
  char str2[] = "";
  char *delim = " ,.-";
  char *s21_token = s21_strtok(str1, delim);
  char *std_token = strtok(str2, delim);
  ck_assert_ptr_eq(s21_token, std_token);
}
END_TEST

START_TEST(strtok3) {
  char str1[] = "\0";
  char str2[] = "\0";
  char *delim = " ,.-";
  char *s21_token = s21_strtok(str1, delim);
  char *std_token = strtok(str2, delim);
  ck_assert_ptr_eq(s21_token, std_token);
}
END_TEST

START_TEST(strtok4) {
  char str1[] = "This is a sample string.";
  char str2[] = "This is a sample string.";
  char *delim = "a";
  char *s21_token = s21_strtok(str1, delim);
  char *std_token = strtok(str2, delim);
  while (s21_token != s21_NULL || std_token != NULL) {
    ck_assert_str_eq(s21_token, std_token);
    s21_token = s21_strtok(s21_NULL, delim);
    std_token = strtok(NULL, delim);
  }
}
END_TEST

START_TEST(to_upper1) {
  char *str = "school";
  char *expected = "SCHOOL";
  char *result = s21_to_upper(str);
  ck_assert_str_eq(result, expected);
  free(result);
}
END_TEST

START_TEST(to_upper2) {
  char *str = "";
  char *expected = "";
  char *result = s21_to_upper(str);
  ck_assert_str_eq(result, expected);
  free(result);
}
END_TEST

START_TEST(to_lower1) {
  char *expected = "school";
  char *str = "SCHOOL";
  char *result = s21_to_lower(str);
  ck_assert_str_eq(result, expected);
  free(result);
}
END_TEST

START_TEST(to_lower2) {
  char *str = "";
  char *expected = "";
  char *result = s21_to_lower(str);
  ck_assert_str_eq(result, expected);
  free(result);
}
END_TEST

START_TEST(trim1) {
  char *str = "  s21 ";
  char *new_str = s21_trim(str, " ");
  char *expected = "s21";
  ck_assert_str_eq(new_str, expected);
  free(new_str);
}
END_TEST

START_TEST(trim2) {
  char *str = "ssss21 ";
  char *new_str = s21_trim(str, "s");
  char *expected = "21 ";
  ck_assert_str_eq(new_str, expected);
  free(new_str);
}
END_TEST

START_TEST(trim3) {
  char *str = "   21  ";
  char *new_str = s21_trim(str, s21_NULL);
  char *expected = "21";
  ck_assert_str_eq(new_str, expected);
  free(new_str);
}
END_TEST

START_TEST(insert) {
  char *str = "21";
  char *src = "school ";
  char *expected = "school 21";
  char *res_ins = s21_insert(src, str, 7);
  ck_assert_str_eq(res_ins, expected);
  free(res_ins);
}
END_TEST

int main(void) {
  Suite *s1 = suite_create("Core");
  TCase *tc1_1 = tcase_create("Core");
  SRunner *sr = srunner_create(s1);
  int nf;

  suite_add_tcase(s1, tc1_1);
  tcase_add_test(tc1_1, strlen1);
  tcase_add_test(tc1_1, strlen2);
  tcase_add_test(tc1_1, memchr1);
  tcase_add_test(tc1_1, memchr2);
  tcase_add_test(tc1_1, memcmp1);
  tcase_add_test(tc1_1, memcmp2);
  tcase_add_test(tc1_1, memcpy1);
  tcase_add_test(tc1_1, memcpy2);
  tcase_add_test(tc1_1, memset1);
  tcase_add_test(tc1_1, memset2);
  tcase_add_test(tc1_1, strncat1);
  tcase_add_test(tc1_1, strncat2);
  tcase_add_test(tc1_1, strncat3);
  tcase_add_test(tc1_1, strncmp1);
  tcase_add_test(tc1_1, strncmp2);
  tcase_add_test(tc1_1, strncmp3);
  tcase_add_test(tc1_1, strncpy1);
  tcase_add_test(tc1_1, strncpy2);
  tcase_add_test(tc1_1, strncpy3);
  tcase_add_test(tc1_1, strrchr1);
  tcase_add_test(tc1_1, strrchr2);
  tcase_add_test(tc1_1, strrchr3);
  tcase_add_test(tc1_1, strrchr4);
  tcase_add_test(tc1_1, strerror1);
  tcase_add_test(tc1_1, strcspn1);
  tcase_add_test(tc1_1, strcspn2);
  tcase_add_test(tc1_1, strcspn3);
  tcase_add_test(tc1_1, strcspn4);
  tcase_add_test(tc1_1, strpbrk1);
  tcase_add_test(tc1_1, strpbrk2);
  tcase_add_test(tc1_1, strstr1);
  tcase_add_test(tc1_1, strstr2);
  tcase_add_test(tc1_1, strtok1);
  tcase_add_test(tc1_1, strtok2);
  tcase_add_test(tc1_1, strtok3);
  tcase_add_test(tc1_1, strtok4);
  tcase_add_test(tc1_1, to_upper1);
  tcase_add_test(tc1_1, to_upper2);
  tcase_add_test(tc1_1, to_lower1);
  tcase_add_test(tc1_1, to_lower2);
  tcase_add_test(tc1_1, trim1);
  tcase_add_test(tc1_1, trim2);
  tcase_add_test(tc1_1, trim3);
  tcase_add_test(tc1_1, insert);

  srunner_run_all(sr, CK_ENV);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
}
