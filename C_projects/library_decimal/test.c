
#include <check.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "s21_decimal.h"

START_TEST(test_s21_from_decimal_to_float_positive) {
  s21_decimal value = {{100, 0, 0, 0}};
  float result;
  int res = s21_from_decimal_to_float(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_float_eq(result, 100.0f);
}
END_TEST

START_TEST(test_s21_from_decimal_to_float_negative) {
  s21_decimal value = {{100, 0, 0, 0x80000000}};
  float result;
  int res = s21_from_decimal_to_float(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_float_eq(result, -100.0f);
}
END_TEST

START_TEST(test_s21_from_decimal_to_float_zero) {
  s21_decimal value = {{0, 0, 0, 0}};
  float result;
  int res = s21_from_decimal_to_float(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_float_eq(result, 0.0f);
}
END_TEST

START_TEST(test_s21_from_decimal_to_float_max_value) {
  s21_decimal value = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  float result;
  int res = s21_from_decimal_to_float(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_float_eq(result, powf(2.0f, 96) - 1);
}
END_TEST

START_TEST(test_s21_from_decimal_to_float_min_value) {
  s21_decimal value = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  float result;
  int res = s21_from_decimal_to_float(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_float_eq(result, -(powf(2.0f, 96) - 1));
}
END_TEST

START_TEST(test_s21_from_decimal_to_float_large_scale) {
  s21_decimal value = {{100, 0, 0, 0x001E0000}};
  float result;
  int res = s21_from_decimal_to_float(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_float_eq(result, 1e-28f);
}
END_TEST

START_TEST(test_s21_from_decimal_to_float_null_dst) {
  s21_decimal value = {{100, 0, 0, 0}};
  int res = s21_from_decimal_to_float(value, NULL);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int_positive) {
  s21_decimal value = {{100, 0, 0, 0}};
  int result;
  int res = s21_from_decimal_to_int(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result, 100);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int_negative) {
  s21_decimal value = {{100, 0, 0, 0x80000000}};
  int result;
  int res = s21_from_decimal_to_int(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result, -100);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int_zero) {
  s21_decimal value = {{0, 0, 0, 0}};
  int result;
  int res = s21_from_decimal_to_int(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int_max_value) {
  s21_decimal value = {{INT_MAX, 0, 0, 0}};
  int result;
  int res = s21_from_decimal_to_int(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result, INT_MAX);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int_min_value) {
  s21_decimal value = {{INT_MIN, 0, 0, 0x80000000}};
  int result;
  int res = s21_from_decimal_to_int(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result, INT_MIN);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int_large_value) {
  s21_decimal value = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  int result;
  int res = s21_from_decimal_to_int(value, &result);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int_null_dst) {
  s21_decimal value = {{100, 0, 0, 0}};
  int res = s21_from_decimal_to_int(value, NULL);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int_large_scale) {
  s21_decimal value = {{100, 0, 0, 0}};
  set_scale(&value, 27);
  int result;
  int res = s21_from_decimal_to_int(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_s21_from_float_to_decimal_positive) {
  float src = 123.456;
  s21_decimal dst;
  int res = s21_from_float_to_decimal(src, &dst);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(dst.bits[0], 123456);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq(get_sign(dst), 0);
  ck_assert_int_eq(get_scale(dst), 3);
}
END_TEST

START_TEST(test_s21_from_float_to_decimal_scale8) {
  float src = 1.23456789;
  s21_decimal dst;
  int res = s21_from_float_to_decimal(src, &dst);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(dst.bits[0], 12345679);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq(get_sign(dst), 0);
  ck_assert_int_eq(get_scale(dst), 7);
}
END_TEST

START_TEST(test_s21_from_float_to_decimal_negative) {
  float src = -123.456;
  s21_decimal dst;
  int res = s21_from_float_to_decimal(src, &dst);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(dst.bits[0], 123456);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq(get_sign(dst), 1);
  ck_assert_int_eq(get_scale(dst), 3);
}
END_TEST

START_TEST(test_s21_from_float_to_decimal_zero) {
  float src = 0.0;
  s21_decimal dst;
  int res = s21_from_float_to_decimal(src, &dst);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(dst.bits[0], 0);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq(get_sign(dst), 0);
  ck_assert_int_eq(get_scale(dst), 0);
}
END_TEST

START_TEST(test_s21_from_float_to_decimal_large) {
  float src = 1e38;
  s21_decimal dst;
  int res = s21_from_float_to_decimal(src, &dst);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_from_float_to_decimal_small) {
  float src = 1e-38;
  s21_decimal dst;
  int res = s21_from_float_to_decimal(src, &dst);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_from_float_to_decimal_nan) {
  float src = NAN;
  s21_decimal dst;
  int res = s21_from_float_to_decimal(src, &dst);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_from_float_to_decimal_inf) {
  float src = INFINITY;
  s21_decimal dst;
  int res = s21_from_float_to_decimal(src, &dst);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_from_int_to_decimal_positive) {
  s21_decimal result = {{0, 0, 0, 0}};
  int src = 123456;
  int res = s21_from_int_to_decimal(src, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 123456);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 0);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_from_int_to_decimal_negative) {
  s21_decimal result = {{0, 0, 0, 0}};
  int src = -123456;
  int res = s21_from_int_to_decimal(src, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 123456);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 1);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_from_int_to_decimal_zero) {
  s21_decimal result = {{0, 0, 0, 0}};
  int src = 0;
  int res = s21_from_int_to_decimal(src, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 0);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_from_int_to_decimal_large_positive) {
  s21_decimal result = {{0, 0, 0, 0}};
  int src = INT_MAX;
  int res = s21_from_int_to_decimal(src, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], INT_MAX);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 0);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_from_int_to_decimal_large_negative) {
  s21_decimal result = {{0, 0, 0, 0}};
  int src = INT_MIN;
  int res = s21_from_int_to_decimal(src, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], (unsigned int)INT_MAX + 1);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 1);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_add_positive) {
  s21_decimal value_1 = {{100, 0, 0, 0}};
  s21_decimal value_2 = {{200, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 300);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

START_TEST(test_s21_add_negative) {
  s21_decimal value_1 = {{100, 0, 0, 0x80000000}};
  s21_decimal value_2 = {{200, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 100);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

START_TEST(test_s21_add_zero) {
  s21_decimal value_1 = {{0, 0, 0, 0}};
  s21_decimal value_2 = {{0, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

START_TEST(test_s21_add_max_value) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{1, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_add_different_scales) {
  s21_decimal value_1 = {{100, 0, 0, 0}};
  s21_decimal value_2 = {{1, 0, 0, 0x00010000}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 1001);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 0x00010000);
}
END_TEST

START_TEST(test_s21_add_negative_and_positive) {
  s21_decimal value_1 = {{100, 0, 0, 0x80000000}};
  s21_decimal value_2 = {{200, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 100);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

START_TEST(test_s21_add_large_numbers) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(res, 1);  // Expecting overflow
}
END_TEST

START_TEST(test_s21_add_small_numbers) {
  s21_decimal value_1 = {{1, 0, 0, 0}};
  s21_decimal value_2 = {{2, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 3);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 0);
}
END_TEST

START_TEST(test_s21_add_with_different_bits) {
  s21_decimal value_1 = {{0, 2, 0, 0}};
  s21_decimal value_2 = {{1, 0, 3, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(result.bits[1], 2);
  ck_assert_int_eq(result.bits[2], 3);
  ck_assert_int_eq(result.bits[3], 0);
}

START_TEST(test_s21_add_with_different_bits_and_scales) {
  s21_decimal value_1 = {{0, 2, 0, 0}};
  s21_decimal value_2 = {{1, 0, 3, 0}};
  set_scale(&value_1, 3);
  set_scale(&value_2, 1);
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 100);
  ck_assert_int_eq(result.bits[1], 2);
  ck_assert_int_eq(result.bits[2], 300);
  ck_assert_int_eq(get_scale(result), 3);
}

START_TEST(test_s21_add_with_different_bits_and_signs) {
  s21_decimal value_1 = {{1, 0, 3, 0}};
  s21_decimal value_2 = {{0, 2, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  set_sign(&value_2, 1);
  int res = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(result.bits[1], MAX_VALUE - 1);
  ck_assert_int_eq(result.bits[2], 2);
  ck_assert_int_eq(result.bits[3], 0);
  ck_assert_int_eq(get_sign(result), 0);
}

START_TEST(test_s21_sub_positive) {
  s21_decimal value_1 = {{100, 0, 0, 0}};
  s21_decimal value_2 = {{50, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 50);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 0);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_sub_negative) {
  s21_decimal value_1 = {{100, 0, 0, 0}};
  set_sign(&value_1, 1);
  s21_decimal value_2 = {{50, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 150);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 1);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_sub_zero) {
  s21_decimal value_1 = {{0, 0, 0, 0}};
  s21_decimal value_2 = {{0, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 0);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_sub_different_scales) {
  s21_decimal value_1 = {{100, 0, 0, 0}};
  set_scale(&value_1, 1);
  s21_decimal value_2 = {{1, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 90);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 0);
  ck_assert_int_eq(get_scale(result), 1);
}
END_TEST

START_TEST(test_s21_sub_negative_and_positive) {
  s21_decimal value_1 = {{100, 0, 0, 0}};
  set_sign(&value_1, 1);
  s21_decimal value_2 = {{50, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 150);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 1);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_sub_small_numbers) {
  s21_decimal value_1 = {{1, 0, 0, 0}};
  s21_decimal value_2 = {{2, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 1);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_sub_different_bits) {
  s21_decimal value_1 = {{0, 0, 1, 0}};
  s21_decimal value_2 = {{0, 0, 2, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 1);
  ck_assert_int_eq(get_sign(result), 1);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_sub_max_different_bits) {
  s21_decimal value_1 = {{MAX_VALUE, MAX_VALUE, MAX_VALUE, 0}};
  s21_decimal value_2 = {{0, 0, 1, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], MAX_VALUE);
  ck_assert_int_eq(result.bits[1], MAX_VALUE);
  ck_assert_int_eq(result.bits[2], MAX_VALUE - 1);
  ck_assert_int_eq(get_sign(result), 0);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_sub_max_values) {
  s21_decimal value_1 = {{MAX_VALUE, MAX_VALUE, MAX_VALUE, 0}};
  s21_decimal value_2 = {{MAX_VALUE, MAX_VALUE, MAX_VALUE, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 0);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_sub_floor_overflow) {
  s21_decimal value_1 = {{MAX_VALUE, MAX_VALUE, MAX_VALUE, 0}};
  s21_decimal value_2 = {{0, 0, MAX_VALUE, 0}};
  set_sign(&value_1, 1);
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(res, 2);  // overflow
}
END_TEST

START_TEST(test_s21_sub_up_overflow) {
  s21_decimal value_1 = {{MAX_VALUE, MAX_VALUE, MAX_VALUE, 0}};
  s21_decimal value_2 = {{0, 0, MAX_VALUE, 0}};
  set_sign(&value_2, 1);
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(res, 1);  // overflow
}
END_TEST

START_TEST(test_s21_mul_positive) {
  s21_decimal value_1 = {{10, 0, 0, 0}};
  s21_decimal value_2 = {{20, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 200);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 0);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_mul_negative) {
  s21_decimal value_1 = {{10, 0, 0, 0}};
  set_sign(&value_1, 1);
  s21_decimal value_2 = {{20, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 200);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 1);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_mul_zero) {
  s21_decimal value_1 = {{0, 0, 0, 0}};
  s21_decimal value_2 = {{20, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 0);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_mul_small_numbers) {
  s21_decimal value_1 = {{1, 0, 0, 0}};
  s21_decimal value_2 = {{2, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 2);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 0);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_mul_all_bits) {
  s21_decimal value_1 = {{10, 30, 10, 0}};
  set_sign(&value_1, 1);
  s21_decimal value_2 = {{20, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 200);
  ck_assert_int_eq(result.bits[1], 600);
  ck_assert_int_eq(result.bits[2], 200);
  ck_assert_int_eq(get_sign(result), 1);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_mul_floor_overflow) {
  s21_decimal value_1 = {{10, 30, MAX_VALUE, 0}};
  set_sign(&value_1, 1);
  s21_decimal value_2 = {{MAX_VALUE, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(res, 2);
}
END_TEST

START_TEST(test_s21_mul_up_overflow) {
  s21_decimal value_1 = {{10, 30, MAX_VALUE, 0}};
  s21_decimal value_2 = {{MAX_VALUE, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_equal_positive) {
  s21_decimal value_1 = {{10, 0, 0, 0}};
  s21_decimal value_2 = {{10, 0, 0, 0}};
  int res = s21_is_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_equal_negative) {
  s21_decimal value_1 = {{10, 0, 0, 0}};
  set_sign(&value_1, 1);
  s21_decimal value_2 = {{10, 0, 0, 0}};
  set_sign(&value_2, 1);
  int res = s21_is_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_equal_different_signs) {
  s21_decimal value_1 = {{10, 0, 0, 0}};
  set_sign(&value_1, 1);
  s21_decimal value_2 = {{10, 0, 0, 0}};
  int res = s21_is_equal(value_1, value_2);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_is_equal_different_scales) {
  s21_decimal value_1 = {{10, 0, 0, 0}};
  set_scale(&value_1, 1);
  s21_decimal value_2 = {{10, 0, 0, 0}};
  int res = s21_is_equal(value_1, value_2);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_is_equal_different_values) {
  s21_decimal value_1 = {{10, 0, 0, 0}};
  s21_decimal value_2 = {{20, 0, 0, 0}};
  int res = s21_is_equal(value_1, value_2);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_is_equal_zero) {
  s21_decimal value_1 = {{0, 0, 0, 0}};
  s21_decimal value_2 = {{0, 0, 0, 0}};
  int res = s21_is_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_equal_large_numbers) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  int res = s21_is_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_equal_small_numbers) {
  s21_decimal value_1 = {{1, 0, 0, 0}};
  s21_decimal value_2 = {{1, 0, 0, 0}};
  int res = s21_is_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_greater_positive) {
  s21_decimal value_1 = {{20, 0, 0, 0}};
  s21_decimal value_2 = {{10, 0, 0, 0}};
  int res = s21_is_greater(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_greater_negative) {
  s21_decimal value_1 = {{20, 0, 0, 0}};
  set_sign(&value_1, 1);
  s21_decimal value_2 = {{10, 0, 0, 0}};
  set_sign(&value_2, 1);
  int res = s21_is_greater(value_1, value_2);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_is_greater_different_signs) {
  s21_decimal value_1 = {{20, 0, 0, 0}};
  s21_decimal value_2 = {{10, 0, 0, 0}};
  set_sign(&value_2, 1);
  int res = s21_is_greater(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_greater_different_scales) {
  s21_decimal value_1 = {{10, 0, 0, 0}};
  set_scale(&value_1, 1);
  s21_decimal value_2 = {{1, 0, 0, 0}};
  int res = s21_is_greater(value_1, value_2);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_is_greater_equal_values) {
  s21_decimal value_1 = {{10, 0, 0, 0}};
  s21_decimal value_2 = {{10, 0, 0, 0}};
  int res = s21_is_greater(value_1, value_2);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_is_greater_zero) {
  s21_decimal value_1 = {{0, 0, 0, 0}};
  s21_decimal value_2 = {{0, 0, 0, 0}};
  int res = s21_is_greater(value_1, value_2);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_is_greater_large_numbers) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  int res = s21_is_greater(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_greater_small_numbers) {
  s21_decimal value_1 = {{2, 0, 0, 0}};
  s21_decimal value_2 = {{1, 0, 0, 0}};
  int res = s21_is_greater(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_greater_different_bits) {
  s21_decimal value_1 = {{0, 0, 1, 0}};
  s21_decimal value_2 = {{3, 3, 0, 0}};
  int res = s21_is_greater(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_greater_with_different_bits) {
  s21_decimal value_1 = {{3, 3, 0, 0}};
  s21_decimal value_2 = {{0, 0, 1, 0}};
  int res = s21_is_greater(value_1, value_2);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_is_greater_or_equal_positive) {
  s21_decimal value_1 = {{20, 0, 0, 0}};
  s21_decimal value_2 = {{10, 0, 0, 0}};
  int res = s21_is_greater_or_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_greater_or_equal_negative) {
  s21_decimal value_1 = {{20, 0, 0, 0}};
  set_sign(&value_1, 1);
  s21_decimal value_2 = {{10, 0, 0, 0}};
  set_sign(&value_2, 1);
  int res = s21_is_greater_or_equal(value_1, value_2);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_is_greater_or_equal_different_signs) {
  s21_decimal value_1 = {{20, 0, 0, 0}};
  s21_decimal value_2 = {{10, 0, 0, 0}};
  set_sign(&value_2, 1);
  int res = s21_is_greater_or_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_greater_or_equal_different_scales) {
  s21_decimal value_1 = {{10, 0, 0, 0}};
  set_scale(&value_1, 1);
  s21_decimal value_2 = {{1, 0, 0, 0}};
  int res = s21_is_greater_or_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_greater_or_equal_equal_values) {
  s21_decimal value_1 = {{10, 0, 0, 0}};
  s21_decimal value_2 = {{10, 0, 0, 0}};
  int res = s21_is_greater_or_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_greater_or_equal_zero) {
  s21_decimal value_1 = {{0, 0, 0, 0}};
  s21_decimal value_2 = {{0, 0, 0, 0}};
  int res = s21_is_greater_or_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_greater_or_equal_large_numbers) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  int res = s21_is_greater_or_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_greater_or_equal_small_numbers) {
  s21_decimal value_1 = {{2, 0, 0, 0}};
  s21_decimal value_2 = {{1, 0, 0, 0}};
  int res = s21_is_greater_or_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_less_positive) {
  s21_decimal value_1 = {{10, 0, 0, 0}};
  s21_decimal value_2 = {{20, 0, 0, 0}};
  int res = s21_is_less(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_less_negative) {
  s21_decimal value_1 = {{10, 0, 0, 0}};
  set_sign(&value_1, 1);
  s21_decimal value_2 = {{20, 0, 0, 0}};
  set_sign(&value_2, 1);
  int res = s21_is_less(value_1, value_2);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_is_less_different_signs) {
  s21_decimal value_1 = {{10, 0, 0, 0}};
  s21_decimal value_2 = {{20, 0, 0, 0}};
  set_sign(&value_2, 1);
  int res = s21_is_less(value_1, value_2);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_is_less_different_scales) {
  s21_decimal value_1 = {{10, 0, 0, 0}};
  set_scale(&value_1, 1);
  s21_decimal value_2 = {{1, 0, 0, 0}};
  int res = s21_is_less(value_1, value_2);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_is_less_equal_values) {
  s21_decimal value_1 = {{10, 0, 0, 0}};
  s21_decimal value_2 = {{10, 0, 0, 0}};
  int res = s21_is_less(value_1, value_2);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_is_less_zero) {
  s21_decimal value_1 = {{0, 0, 0, 0}};
  s21_decimal value_2 = {{0, 0, 0, 0}};
  int res = s21_is_less(value_1, value_2);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_is_less_large_numbers) {
  s21_decimal value_1 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  int res = s21_is_less(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_less_small_numbers) {
  s21_decimal value_1 = {{1, 0, 0, 0}};
  s21_decimal value_2 = {{2, 0, 0, 0}};
  int res = s21_is_less(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_not_equal_positive) {
  s21_decimal value_1 = {{10, 0, 0, 0}};
  s21_decimal value_2 = {{20, 0, 0, 0}};
  int res = s21_is_not_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_not_equal_negative) {
  s21_decimal value_1 = {{10, 0, 0, 0}};
  set_sign(&value_1, 1);
  s21_decimal value_2 = {{10, 0, 0, 0}};
  int res = s21_is_not_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_not_equal_different_signs) {
  s21_decimal value_1 = {{10, 0, 0, 0}};
  set_sign(&value_1, 1);
  s21_decimal value_2 = {{10, 0, 0, 0}};
  set_sign(&value_2, 0);
  int res = s21_is_not_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_not_equal_different_scales) {
  s21_decimal value_1 = {{10, 0, 0, 0}};
  set_scale(&value_1, 1);
  s21_decimal value_2 = {{10, 0, 0, 0}};
  int res = s21_is_not_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_not_equal_equal_values) {
  s21_decimal value_1 = {{10, 0, 0, 0}};
  s21_decimal value_2 = {{10, 0, 0, 0}};
  int res = s21_is_not_equal(value_1, value_2);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_is_not_equal_zero) {
  s21_decimal value_1 = {{0, 0, 0, 0}};
  s21_decimal value_2 = {{0, 0, 0, 0}};
  int res = s21_is_not_equal(value_1, value_2);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_is_not_equal_large_numbers) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  int res = s21_is_not_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_not_equal_small_numbers) {
  s21_decimal value_1 = {{1, 0, 0, 0}};
  s21_decimal value_2 = {{2, 0, 0, 0}};
  int res = s21_is_not_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_less_or_equal_positive) {
  s21_decimal value_1 = {{10, 0, 0, 0}};
  s21_decimal value_2 = {{20, 0, 0, 0}};
  int res = s21_is_less_or_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_less_or_equal_negative) {
  s21_decimal value_1 = {{10, 0, 0, 0}};
  set_sign(&value_1, 1);
  s21_decimal value_2 = {{20, 0, 0, 0}};
  set_sign(&value_2, 1);
  int res = s21_is_less_or_equal(value_1, value_2);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_is_less_or_equal_different_signs) {
  s21_decimal value_1 = {{10, 0, 0, 0}};
  s21_decimal value_2 = {{20, 0, 0, 0}};
  set_sign(&value_2, 1);
  int res = s21_is_less_or_equal(value_1, value_2);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(test_s21_is_less_or_equal_different_scales) {
  s21_decimal value_1 = {{10, 0, 0, 0}};
  set_scale(&value_1, 1);
  s21_decimal value_2 = {{1, 0, 0, 0}};
  int res = s21_is_less_or_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_less_or_equal_equal_values) {
  s21_decimal value_1 = {{10, 0, 0, 0}};
  s21_decimal value_2 = {{10, 0, 0, 0}};
  int res = s21_is_less_or_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_less_or_equal_zero) {
  s21_decimal value_1 = {{0, 0, 0, 0}};
  s21_decimal value_2 = {{0, 0, 0, 0}};
  int res = s21_is_less_or_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_less_or_equal_large_numbers) {
  s21_decimal value_1 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  int res = s21_is_less_or_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_less_or_equal_different_bits) {
  s21_decimal value_1 = {{0, 0, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  int res = s21_is_less_or_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_less_or_equal_different_bits_negative) {
  s21_decimal value_1 = {{0, 0, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  set_sign(&value_1, 1);
  set_sign(&value_2, 0);
  int res = s21_is_less_or_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_is_less_or_equal_small_numbers) {
  s21_decimal value_1 = {{1, 0, 0, 0}};
  s21_decimal value_2 = {{2, 0, 0, 0}};
  int res = s21_is_less_or_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_negate_positive) {
  s21_decimal value = {{10, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_negate(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 10);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 1);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_negate_negative) {
  s21_decimal value = {{10, 0, 0, 0}};
  set_sign(&value, 1);
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_negate(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 10);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 0);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_negate_zero) {
  s21_decimal value = {{0, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_negate(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 1);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_negate_small_number) {
  s21_decimal value = {{1, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_negate(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 1);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_negate_large_number) {
  s21_decimal value = {{MAX_VALUE, MAX_VALUE, MAX_VALUE, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_negate(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], MAX_VALUE);
  ck_assert_int_eq(result.bits[1], MAX_VALUE);
  ck_assert_int_eq(result.bits[2], MAX_VALUE);
  ck_assert_int_eq(get_sign(result), 1);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_negate_with_scale) {
  s21_decimal value = {{MAX_VALUE, 0, MAX_VALUE, 0}};
  set_scale(&value, 10);
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_negate(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], MAX_VALUE);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], MAX_VALUE);
  ck_assert_int_eq(get_sign(result), 1);
  ck_assert_int_eq(get_scale(result), 10);
}
END_TEST

START_TEST(test_s21_negate_big) {
  s21_decimal value = {{2812812, 528, 0, 0}};
  set_sign(&value, 1);
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_negate(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 2812812);
  ck_assert_int_eq(result.bits[1], 528);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 0);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_round_positive) {
  s21_decimal value = {{123456, 0, 0, 0}};
  set_scale(&value, 2);
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_round(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 1235);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 0);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_round_negative) {
  s21_decimal value = {{123456, 0, 0, 0}};
  set_sign(&value, 1);
  set_scale(&value, 2);
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_round(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 1235);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 1);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_round_zero) {
  s21_decimal value = {{0, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_round(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 0);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_round_small_number) {
  s21_decimal value = {{1, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_round(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 0);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_round_negative_num) {
  s21_decimal value = {{123777, 0, 0, 0}};
  set_sign(&value, 1);
  set_scale(&value, 2);
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_round(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 1238);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 1);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_round_big) {
  s21_decimal value = {{2812812, 528, 0, 0}};
  set_scale(&value, 7);
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_round(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 226775);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 0);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_truncate_positive) {
  s21_decimal value = {{123456, 0, 0, 0}};
  set_scale(&value, 2);
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_truncate(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 1234);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 0);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_truncate_big) {
  s21_decimal value = {{2812812, 528, 0, 0}};
  set_scale(&value, 7);
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_truncate(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 226774);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 0);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_truncate_negative) {
  s21_decimal value = {{123456, 0, 0, 0}};
  set_sign(&value, 1);
  set_scale(&value, 3);
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_truncate(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 123);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 1);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_truncate_zero) {
  s21_decimal value = {{0, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_truncate(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 0);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_truncate_small_number) {
  s21_decimal value = {{1, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_truncate(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 0);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_truncate_large) {
  s21_decimal value = {{24, 23, 0, 0}};
  set_scale(&value, 1);
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_truncate(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 1288490191);
  ck_assert_int_eq(result.bits[1], 2);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 0);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_floor_positive) {
  s21_decimal value = {{123456, 0, 0, 0}};
  set_scale(&value, 3);
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_floor(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 123);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 0);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_floor_negative) {
  s21_decimal value = {{123456, 0, 0, 0}};
  set_sign(&value, 1);
  set_scale(&value, 2);
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_floor(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 1235);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 1);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_floor_zero) {
  s21_decimal value = {{0, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_floor(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 0);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_floor_small_number) {
  s21_decimal value = {{7, 0, 0, 0}};
  set_sign(&value, 1);
  set_scale(&value, 1);
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_floor(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 1);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_floor_big) {
  s21_decimal value = {{2812812, 528, 0, 0}};
  set_scale(&value, 7);
  set_sign(&value, 1);
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_floor(value, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 226775);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 1);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(test_s21_div_positive) {
  s21_decimal value_1 = {{4, 0, 0, 0}};
  s21_decimal value_2 = {{2, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 2);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(get_sign(result), 0);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

Suite *s21_decimal_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_decimal");

  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_from_decimal_to_float_positive);
  tcase_add_test(tc_core, test_s21_from_decimal_to_float_negative);
  tcase_add_test(tc_core, test_s21_from_decimal_to_float_zero);
  tcase_add_test(tc_core, test_s21_from_decimal_to_float_max_value);
  tcase_add_test(tc_core, test_s21_from_decimal_to_float_min_value);
  tcase_add_test(tc_core, test_s21_from_decimal_to_float_large_scale);
  tcase_add_test(tc_core, test_s21_from_decimal_to_float_null_dst);
  tcase_add_test(tc_core, test_s21_from_decimal_to_int_positive);
  tcase_add_test(tc_core, test_s21_from_decimal_to_int_negative);
  tcase_add_test(tc_core, test_s21_from_decimal_to_int_zero);
  tcase_add_test(tc_core, test_s21_from_decimal_to_int_max_value);
  tcase_add_test(tc_core, test_s21_from_decimal_to_int_min_value);
  tcase_add_test(tc_core, test_s21_from_decimal_to_int_large_value);
  tcase_add_test(tc_core, test_s21_from_decimal_to_int_null_dst);
  tcase_add_test(tc_core, test_s21_from_decimal_to_int_large_scale);
  tcase_add_test(tc_core, test_s21_from_float_to_decimal_zero);
  tcase_add_test(tc_core, test_s21_from_float_to_decimal_positive);
  tcase_add_test(tc_core, test_s21_from_float_to_decimal_negative);
  tcase_add_test(tc_core, test_s21_from_float_to_decimal_zero);
  tcase_add_test(tc_core, test_s21_from_float_to_decimal_large);
  tcase_add_test(tc_core, test_s21_from_float_to_decimal_small);
  tcase_add_test(tc_core, test_s21_from_float_to_decimal_nan);
  tcase_add_test(tc_core, test_s21_from_float_to_decimal_inf);
  tcase_add_test(tc_core, test_s21_from_float_to_decimal_positive);
  tcase_add_test(tc_core, test_s21_from_float_to_decimal_negative);
  tcase_add_test(tc_core, test_s21_from_float_to_decimal_zero);
  tcase_add_test(tc_core, test_s21_from_float_to_decimal_large);
  tcase_add_test(tc_core, test_s21_from_float_to_decimal_small);
  tcase_add_test(tc_core, test_s21_from_int_to_decimal_positive);
  tcase_add_test(tc_core, test_s21_from_int_to_decimal_negative);
  tcase_add_test(tc_core, test_s21_from_int_to_decimal_zero);
  tcase_add_test(tc_core, test_s21_from_int_to_decimal_large_positive);
  tcase_add_test(tc_core, test_s21_from_int_to_decimal_large_negative);
  tcase_add_test(tc_core, test_s21_add_positive);
  tcase_add_test(tc_core, test_s21_add_negative);
  tcase_add_test(tc_core, test_s21_add_zero);
  tcase_add_test(tc_core, test_s21_add_max_value);
  tcase_add_test(tc_core, test_s21_add_different_scales);
  tcase_add_test(tc_core, test_s21_add_negative_and_positive);
  tcase_add_test(tc_core, test_s21_add_large_numbers);
  tcase_add_test(tc_core, test_s21_add_small_numbers);
  tcase_add_test(tc_core, test_s21_sub_positive);
  tcase_add_test(tc_core, test_s21_sub_negative);
  tcase_add_test(tc_core, test_s21_sub_zero);
  tcase_add_test(tc_core, test_s21_sub_different_scales);
  tcase_add_test(tc_core, test_s21_sub_negative_and_positive);
  tcase_add_test(tc_core, test_s21_sub_small_numbers);
  tcase_add_test(tc_core, test_s21_mul_positive);
  tcase_add_test(tc_core, test_s21_mul_negative);
  tcase_add_test(tc_core, test_s21_mul_zero);
  tcase_add_test(tc_core, test_s21_mul_small_numbers);
  tcase_add_test(tc_core, test_s21_is_equal_positive);
  tcase_add_test(tc_core, test_s21_is_equal_negative);
  tcase_add_test(tc_core, test_s21_is_equal_different_signs);
  tcase_add_test(tc_core, test_s21_is_equal_different_scales);
  tcase_add_test(tc_core, test_s21_is_equal_different_values);
  tcase_add_test(tc_core, test_s21_is_equal_zero);
  tcase_add_test(tc_core, test_s21_is_equal_large_numbers);
  tcase_add_test(tc_core, test_s21_is_equal_small_numbers);
  tcase_add_test(tc_core, test_s21_is_greater_positive);
  tcase_add_test(tc_core, test_s21_is_greater_negative);
  tcase_add_test(tc_core, test_s21_is_greater_different_signs);
  tcase_add_test(tc_core, test_s21_is_greater_different_scales);
  tcase_add_test(tc_core, test_s21_is_greater_equal_values);
  tcase_add_test(tc_core, test_s21_is_greater_zero);
  tcase_add_test(tc_core, test_s21_is_greater_large_numbers);
  tcase_add_test(tc_core, test_s21_is_greater_small_numbers);
  tcase_add_test(tc_core, test_s21_is_greater_or_equal_positive);
  tcase_add_test(tc_core, test_s21_is_greater_or_equal_negative);
  tcase_add_test(tc_core, test_s21_is_greater_or_equal_different_signs);
  tcase_add_test(tc_core, test_s21_is_greater_or_equal_different_scales);
  tcase_add_test(tc_core, test_s21_is_greater_or_equal_equal_values);
  tcase_add_test(tc_core, test_s21_is_greater_or_equal_zero);
  tcase_add_test(tc_core, test_s21_is_greater_or_equal_large_numbers);
  tcase_add_test(tc_core, test_s21_is_greater_or_equal_small_numbers);
  tcase_add_test(tc_core, test_s21_is_less_positive);
  tcase_add_test(tc_core, test_s21_is_less_negative);
  tcase_add_test(tc_core, test_s21_is_less_different_signs);
  tcase_add_test(tc_core, test_s21_is_less_different_scales);
  tcase_add_test(tc_core, test_s21_is_less_equal_values);
  tcase_add_test(tc_core, test_s21_is_less_zero);
  tcase_add_test(tc_core, test_s21_is_less_large_numbers);
  tcase_add_test(tc_core, test_s21_is_less_small_numbers);
  tcase_add_test(tc_core, test_s21_is_not_equal_positive);
  tcase_add_test(tc_core, test_s21_is_not_equal_negative);
  tcase_add_test(tc_core, test_s21_is_not_equal_different_signs);
  tcase_add_test(tc_core, test_s21_is_not_equal_different_scales);
  tcase_add_test(tc_core, test_s21_is_not_equal_equal_values);
  tcase_add_test(tc_core, test_s21_is_not_equal_zero);
  tcase_add_test(tc_core, test_s21_is_not_equal_large_numbers);
  tcase_add_test(tc_core, test_s21_is_not_equal_small_numbers);
  tcase_add_test(tc_core, test_s21_is_less_or_equal_positive);
  tcase_add_test(tc_core, test_s21_is_less_or_equal_negative);
  tcase_add_test(tc_core, test_s21_is_less_or_equal_different_signs);
  tcase_add_test(tc_core, test_s21_is_less_or_equal_different_scales);
  tcase_add_test(tc_core, test_s21_is_less_or_equal_equal_values);
  tcase_add_test(tc_core, test_s21_is_less_or_equal_zero);
  tcase_add_test(tc_core, test_s21_is_less_or_equal_large_numbers);
  tcase_add_test(tc_core, test_s21_is_less_or_equal_small_numbers);
  tcase_add_test(tc_core, test_s21_negate_positive);
  tcase_add_test(tc_core, test_s21_negate_negative);
  tcase_add_test(tc_core, test_s21_negate_zero);
  tcase_add_test(tc_core, test_s21_negate_small_number);
  tcase_add_test(tc_core, test_s21_round_positive);
  tcase_add_test(tc_core, test_s21_round_negative);
  tcase_add_test(tc_core, test_s21_round_zero);
  tcase_add_test(tc_core, test_s21_round_small_number);
  tcase_add_test(tc_core, test_s21_truncate_positive);
  tcase_add_test(tc_core, test_s21_truncate_negative);
  tcase_add_test(tc_core, test_s21_truncate_zero);
  tcase_add_test(tc_core, test_s21_truncate_small_number);
  tcase_add_test(tc_core, test_s21_floor_positive);
  tcase_add_test(tc_core, test_s21_floor_negative);
  tcase_add_test(tc_core, test_s21_floor_zero);
  tcase_add_test(tc_core, test_s21_floor_small_number);
  tcase_add_test(tc_core, test_s21_div_positive);
  tcase_add_test(tc_core, test_s21_add_with_different_bits);
  tcase_add_test(tc_core, test_s21_add_with_different_bits_and_scales);
  tcase_add_test(tc_core, test_s21_add_with_different_bits_and_signs);
  tcase_add_test(tc_core, test_s21_sub_different_bits);
  tcase_add_test(tc_core, test_s21_sub_max_values);
  tcase_add_test(tc_core, test_s21_sub_max_different_bits);
  tcase_add_test(tc_core, test_s21_sub_floor_overflow);
  tcase_add_test(tc_core, test_s21_sub_up_overflow);
  tcase_add_test(tc_core, test_s21_mul_all_bits);
  tcase_add_test(tc_core, test_s21_mul_floor_overflow);
  tcase_add_test(tc_core, test_s21_mul_up_overflow);
  tcase_add_test(tc_core, test_s21_is_greater_with_different_bits);
  tcase_add_test(tc_core, test_s21_is_greater_different_bits);
  tcase_add_test(tc_core, test_s21_is_less_or_equal_different_bits);
  tcase_add_test(tc_core, test_s21_is_less_or_equal_different_bits_negative);
  tcase_add_test(tc_core, test_s21_negate_large_number);
  tcase_add_test(tc_core, test_s21_negate_with_scale);
  tcase_add_test(tc_core, test_s21_round_negative_num);
  tcase_add_test(tc_core, test_s21_from_float_to_decimal_scale8);
  tcase_add_test(tc_core, test_s21_truncate_large);
  tcase_add_test(tc_core, test_s21_round_big);
  tcase_add_test(tc_core, test_s21_truncate_big);
  tcase_add_test(tc_core, test_s21_floor_big);
  tcase_add_test(tc_core, test_s21_negate_big);
  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = s21_decimal_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return number_failed;
}
