#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
  unsigned int bits[8];
} s21_big_decimal;
typedef struct {
  unsigned int bits[4];
} s21_decimal;

#define MINUS 0x80000000
#define MAX_VALUE (0b11111111111111111111111111111111)

void s21_big_add(s21_big_decimal value_1, s21_big_decimal value_2,
                 s21_big_decimal* result);
void s21_big_sub(s21_big_decimal value_1, s21_big_decimal value_2,
                 s21_big_decimal* result);
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
int check_not_zero(s21_big_decimal value);
void shift_right(s21_big_decimal* reserve, int count);
void set_big_bit_with_shift(s21_big_decimal* dst, int degree, int shift,
                            int bit);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
int s21_from_big_to_dec(s21_big_decimal value_1, s21_decimal* result);
int get_big_sign(s21_big_decimal value);
void set_big_sign(s21_big_decimal* value, int sign_value);
int get_big_scale(s21_big_decimal value);
void set_big_scale(s21_big_decimal* value, int scale_value);
int get_big_bit(s21_big_decimal value, int bit_num);
void set_big_bit(s21_big_decimal* value, int bit_num, unsigned val);
int get_sign(s21_decimal value);
void set_sign(s21_decimal* value, int sign_value);
int get_scale(s21_decimal value);
void set_scale(s21_decimal* value, int scale_value);
int get_bit(s21_decimal value, int bit_num);
void set_bit(s21_decimal* value, int bit_num, unsigned val);
void shift_left(s21_decimal* value);
void big_shift_left(s21_big_decimal* value);
void equal_big_scale(s21_big_decimal* value_1, s21_big_decimal* value_2);
void big_multiply_to_10(s21_big_decimal* value, int* value_scale);
int s21_is_less(s21_decimal value_1, s21_decimal value_2);
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2);
int compare(s21_big_decimal value_1, s21_big_decimal value_2);
void equal_scale_for_compare(s21_decimal value_1, s21_decimal value_2,
                             s21_big_decimal* value_11,
                             s21_big_decimal* value_22);
void big_to_decimal(s21_decimal* value, s21_big_decimal big_value);
void divide_by_10_with_banking_rounding(s21_big_decimal* num);
bool is_mantissa_overflow(s21_big_decimal* num);
void reduce_scale(s21_big_decimal* num);
int normalize(s21_big_decimal* num);
void decimal_to_big(s21_decimal value, s21_big_decimal* big_value);
void print_decimal(s21_decimal value);
int s21_decimal_correct(s21_decimal value);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
void s21_big_mul(s21_big_decimal value_1, s21_big_decimal value_2,
                 s21_big_decimal* result);
void big_shift_left_to_i(s21_big_decimal* value, int i);
int overflow_for_mul(s21_big_decimal value);
void result_for_mul(s21_big_decimal* big_result, int big_res_sign, int* res,
                    s21_decimal* result);
int s21_big_div(s21_big_decimal value_1, s21_big_decimal value_2,
                s21_big_decimal* result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);

int s21_negate(s21_decimal value, s21_decimal* result);
int s21_truncate(s21_decimal value, s21_decimal* result);
int s21_round(s21_decimal value, s21_decimal* result);
int s21_floor(s21_decimal value, s21_decimal* result);

void big_shift_right(s21_big_decimal num, int moving, s21_big_decimal* result);
s21_big_decimal div_by_ten_to_res(s21_big_decimal* value);

void s21_decl_to_null(s21_decimal* decl);
void s21_big_decl_to_null(s21_big_decimal* decl);

int s21_from_int_to_decimal(int src, s21_decimal* dst);
int s21_from_float_to_decimal(float src, s21_decimal* dst);
int s21_from_decimal_to_int(s21_decimal src, int* dst);
void remove_last_digit(s21_decimal* num);
int s21_from_decimal_to_float(s21_decimal src, float* dst);

s21_big_decimal big_divide_by_10(s21_big_decimal* num, s21_big_decimal* ost);
