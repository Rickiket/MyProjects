#include "s21_decimal.h"

#include <ctype.h>
#include <stdio.h>

void s21_big_add(s21_big_decimal value_1, s21_big_decimal value_2,
                 s21_big_decimal* result) {
  unsigned memo = 0;
  for (int i = 0; i < 32 * 7; i++) {
    unsigned result_bit =
        get_big_bit(value_1, i) + get_big_bit(value_2, i) + memo;
    memo = result_bit / 2;
    result_bit %= 2;

    set_big_bit(result, i, result_bit);
  }
}

void s21_big_sub(s21_big_decimal value_1, s21_big_decimal value_2,
                 s21_big_decimal* result) {
  int borrow = 0;
  for (int i = 0; i < 32 * 7; i++) {
    int result_bit = get_big_bit(value_1, i) - get_big_bit(value_2, i) - borrow;
    if (result_bit < 0) {
      result_bit += 2;
      borrow = 1;
    } else {
      borrow = 0;
    }

    set_big_bit(result, i, result_bit);
  }
}
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int res = 0;
  s21_big_decimal value_11 = {0};
  s21_big_decimal value_22 = {0};
  int value_1_sign = get_sign(value_1);
  int value_2_sign = get_sign(value_2);
  if (value_1_sign == value_2_sign) {
    s21_big_decimal big_result = {0};
    equal_scale_for_compare(value_1, value_2, &value_11, &value_22);
    int scale_for_result = get_big_scale(value_11);
    set_big_scale(&big_result, scale_for_result);
    s21_big_add(value_11, value_22, &big_result);

    if (normalize(&big_result)) {
      big_to_decimal(result, big_result);
      if (value_1_sign) {
        set_sign(result, 1);
      }
      res = 0;
    } else {
      if (value_1_sign) {
        set_sign(result, 1);
        res = 2;
      } else {
        res = 1;
      }
    }
  } else {
    if (value_1_sign && value_2_sign == 0) {
      set_sign(&value_1, 0);
      res = s21_sub(value_2, value_1, result);
    } else if (value_1_sign == 0 && value_2_sign) {
      set_sign(&value_2, 0);
      res = s21_sub(value_1, value_2, result);
    }
  }
  return res;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int res = 0;
  int neg_result = 0;
  s21_big_decimal value_11 = {0};
  s21_big_decimal value_22 = {0};
  s21_big_decimal big_result = {0};
  int value_1_sign = get_sign(value_1);
  int value_2_sign = get_sign(value_2);
  if (value_1_sign == value_2_sign) {
    equal_scale_for_compare(value_1, value_2, &value_11, &value_22);
    int scale_for_result = get_big_scale(value_11);
    set_big_scale(&big_result, scale_for_result);
    s21_decimal value_1_copy = value_1;
    s21_decimal value_2_copy = value_2;
    set_sign(&value_1_copy, 0);
    set_sign(&value_2_copy, 0);
    if (s21_is_greater(value_2_copy, value_1_copy)) {
      s21_big_sub(value_22, value_11, &big_result);
      neg_result = 1;
    } else {
      s21_big_sub(value_11, value_22, &big_result);
      if (get_sign(value_1) && get_sign(value_2)) neg_result = 1;
    }
    if (normalize(&big_result)) {
      big_to_decimal(result, big_result);
      if (neg_result) {
        set_sign(result, 1);
      }
      res = 0;
    } else {
      if (value_1_sign) {
        set_sign(result, 1);
        res = 2;
      } else {
        res = 1;
      }
    }
  } else {
    if (value_1_sign == 1 && value_2_sign == 0) {
      set_sign(&value_2, 1);
      res = s21_add(value_2, value_1, result);
      set_sign(result, 1);
    } else {
      set_sign(&value_2, 0);
      set_sign(&value_1, 0);
      res = s21_add(value_1, value_2, result);
    }
  }
  return res;
}
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int res = 0;
  s21_big_decimal value_11 = {0};
  s21_big_decimal value_22 = {0};
  s21_big_decimal big_result = {0};
  decimal_to_big(value_1, &value_11);
  decimal_to_big(value_2, &value_22);

  int big_res_sign = get_sign(value_1) == get_sign(value_2) ? 0 : 1;
  int result_scale = get_scale(value_1) + get_scale(value_2);
  set_big_scale(&big_result, result_scale);
  s21_big_mul(value_11, value_22, &big_result);
  result_for_mul(&big_result, big_res_sign, &res, result);
  return res;
}
void s21_big_mul(s21_big_decimal value_1, s21_big_decimal value_2,
                 s21_big_decimal* result) {
  for (int i = 0; i < 32 * 7; i++) {
    if (get_big_bit(value_1, i)) {
      s21_big_decimal temp = value_2;
      big_shift_left_to_i(&temp, i);
      s21_big_add(*result, temp, result);
    }
  }
}
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  s21_big_decimal big_value_1 = {0};
  s21_big_decimal big_value_2 = {0};
  s21_big_decimal big_result = {0};
  decimal_to_big(value_1, &big_value_1);
  decimal_to_big(value_2, &big_value_2);
  decimal_to_big(*result, &big_result);

  int flag = s21_big_div(big_value_1, big_value_2, &big_result);
  if (flag == 0) {
    if (get_sign(value_1) + get_sign(value_2) == 1) {
      set_big_sign(&big_result, 1);
    }
    flag = s21_from_big_to_dec(big_result, result);
  }
  return flag;
}

int s21_big_div(s21_big_decimal value_1, s21_big_decimal value_2,
                s21_big_decimal* result) {
  int flag = 0;
  s21_big_decimal ten = {0};
  ten.bits[0] = 10;
  set_big_scale(&value_1, 0);
  set_big_scale(&value_2, 0);
  if (check_not_zero(value_2)) {
    flag = 3;
  } else {
    int total_scale = 0;
    while (value_1.bits[0] != 0 && total_scale < 29) {
      int counter = 0;
      s21_big_decimal reserve = {0};
      s21_big_add(reserve, value_2, &reserve);
      while (value_1.bits[0] >= reserve.bits[0]) {
        big_shift_left(&reserve);
        counter++;
      }
      if (value_1.bits[0] >= value_2.bits[0] && counter) {
        shift_right(&reserve, 1);
        counter--;
      } else if (value_2.bits[0] > value_1.bits[0]) {
        s21_big_mul(value_1, ten, &value_1);
        total_scale++;
      }
      if (value_1.bits[0] >= value_2.bits[0]) {
        s21_big_sub(value_1, reserve, &value_1);
      }
      set_big_bit_with_shift(result, 0, counter, 1);
    }
    set_big_scale(result, total_scale);
  }
  return flag;
}
int check_not_zero(s21_big_decimal value) {
  int res = 1;
  for (int i = 0; i < 7 && res; i++) {
    if (value.bits[i]) {
      res = 0;
    }
  }
  return res;
}
void shift_right(s21_big_decimal* reserve, int count) {
  for (int j = 0; j < count; j++) {
    for (int i = 0; i < 7; i++) {
      reserve->bits[i] >>= 1;
      if (i != 2) {
        reserve->bits[i] |= (reserve->bits[i + 1] & 1) << 30;
      }
    }
  }
}
void set_big_bit_with_shift(s21_big_decimal* dst, int degree, int shift,
                            int bit) {
  if (bit) {
    dst->bits[degree] |= bit << shift;
  } else {
    dst->bits[degree] &= ~(bit << shift);
  }
}
int s21_from_big_to_dec(s21_big_decimal value_1, s21_decimal* result) {
  int flag = 0;
  if (value_1.bits[4] || value_1.bits[5] || value_1.bits[6] ||
      value_1.bits[3]) {
    if (get_big_sign(value_1)) {
      flag = 2;
    } else {
      flag = 1;
    }
  } else {
    for (int i = 0; i < 3; i++) {
      result->bits[i] = value_1.bits[i];
    }
    result->bits[3] = value_1.bits[7];
  }
  return flag;
}
int get_big_sign(s21_big_decimal value) {
  unsigned int res = value.bits[7] & 1 << 31;
  return res >> 31;
}
void set_big_sign(s21_big_decimal* value, int sign_value) {
  if (sign_value == 0) {
    value->bits[7] &= ~(1 << 31);
  } else if (sign_value == 1) {
    value->bits[7] |= 1 << 31;
  }
}
int get_sign(s21_decimal value) {
  unsigned int res = value.bits[3] & 1 << 31;
  return res >> 31;
}
void set_sign(s21_decimal* value, int sign_value) {
  if (sign_value == 0) {
    value->bits[3] &= ~(1 << 31);
  } else if (sign_value == 1) {
    value->bits[3] |= 1 << 31;
  }
}
int get_big_scale(s21_big_decimal value) {
  return (value.bits[7] >> 16) & 0xFF;
}
void set_big_scale(s21_big_decimal* value, int scale_value) {
  if (scale_value >= 0 && scale_value <= 28) {
    value->bits[7] &= ~(255 << 16);
    value->bits[7] |= scale_value << 16;
  }
}
int get_scale(s21_decimal value) { return (value.bits[3] >> 16) & 0xFF; }
void set_scale(s21_decimal* value, int scale_value) {
  if (scale_value >= 0 && scale_value <= 28) {
    value->bits[3] &= ~(255 << 16);
    value->bits[3] |= scale_value << 16;
  }
}
int get_big_bit(s21_big_decimal value, int bit_num) {
  unsigned int result = -1;
  if (bit_num >= 0 && bit_num <= 223) {
    unsigned int res = 1 << bit_num % 32;
    res = value.bits[bit_num / 32] & res;
    result = res >> bit_num % 32;
  }
  return result;
}
void set_big_bit(s21_big_decimal* value, int bit_num, unsigned val) {
  if (bit_num >= 0 && bit_num <= 223) {
    unsigned int res = 1 << bit_num % 32;
    if (val == 1) {
      value->bits[bit_num / 32] |= res;
    } else if (val == 0) {
      value->bits[bit_num / 32] &= ~res;
    }
  }
}
int get_bit(s21_decimal value, int bit_num) {
  unsigned int result = -1;
  if (bit_num >= 0 && bit_num <= 223) {
    unsigned int res = 1 << bit_num % 32;
    res = value.bits[bit_num / 32] & res;
    result = res >> bit_num % 32;
  }
  return result;
}
void set_bit(s21_decimal* value, int bit_num, unsigned val) {
  if (bit_num >= 0 && bit_num <= 223) {
    unsigned int res = 1 << bit_num % 32;
    if (val == 1) {
      value->bits[bit_num / 32] |= res;
    } else if (val == 0) {
      value->bits[bit_num / 32] &= ~res;
    }
  }
}
void shift_left(s21_decimal* value) {
  unsigned memo = 0;
  for (int i = 0; i < 3; i++) {
    unsigned temp = value->bits[i];
    value->bits[i] <<= 1;
    value->bits[i] |= memo;
    memo = temp >> 31;
  }
}
void big_shift_left(s21_big_decimal* value) {
  unsigned memo = 0;
  for (int i = 0; i < 7; i++) {
    unsigned temp = value->bits[i];
    value->bits[i] <<= 1;
    value->bits[i] |= memo;
    memo = temp >> 31;
  }
}
void big_shift_left_to_i(s21_big_decimal* value, int i) {
  for (int j = 0; j < i; j++) {
    big_shift_left(value);
  }
}
void decimal_to_big(s21_decimal value, s21_big_decimal* big_value) {
  for (int i = 0; i < 3; i++) {
    big_value->bits[i] = value.bits[i];
  }
  big_value->bits[7] = value.bits[3];
}
void big_to_decimal(s21_decimal* value, s21_big_decimal big_value) {
  for (int i = 0; i < 3; i++) {
    value->bits[i] = big_value.bits[i];
  }
  value->bits[3] = big_value.bits[7];
}
void equal_big_scale(s21_big_decimal* value_1, s21_big_decimal* value_2) {
  int value_1_scale = get_big_scale(*value_1);
  int value_2_scale = get_big_scale(*value_2);
  if (value_1_scale < value_2_scale) {
    while (value_1_scale != value_2_scale && value_1_scale < 28) {
      big_multiply_to_10(value_1, &value_1_scale);
    }
  } else {
    while (value_2_scale != value_1_scale && value_2_scale < 28) {
      big_multiply_to_10(value_2, &value_2_scale);
    }
  }
}
void big_multiply_to_10(s21_big_decimal* value, int* value_scale) {
  s21_big_decimal temp = *value;
  big_shift_left_to_i(value, 3);
  big_shift_left(&temp);
  s21_big_add(temp, *value, value);
  *value_scale += 1;
  set_big_scale(value, *value_scale);
}
void divide_by_10_with_banking_rounding(s21_big_decimal* num) {
  unsigned long long remainder = 0;
  for (int i = 6; i >= 0; i--) {
    unsigned long value = (remainder << 32) + num->bits[i];
    num->bits[i] = (value / 10);
    remainder = value % 10;
  }
  if (remainder > 5 || (remainder == 5 && (num->bits[0] & 1))) {
    num->bits[0] += 1;
    for (int i = 0; i < 7; i++) {
      if (num->bits[i] == 0) {
        num->bits[i + 1] += 1;
      } else {
        i = 8;
      }
    }
  }
}

s21_big_decimal big_divide_by_10(s21_big_decimal* num, s21_big_decimal* ost) {
  s21_big_decimal res = *num;
  s21_decimal rem = {0};
  s21_big_decimal rem_big = {0};
  s21_big_decl_to_null(num);
  unsigned long long remainder = 0;
  for (int i = 6; i >= 0; i--) {
    unsigned long value = (remainder << 32) + res.bits[i];
    res.bits[i] = (value / 10);
    remainder = value % 10;
  }
  s21_from_int_to_decimal(remainder, &rem);
  decimal_to_big(rem, &rem_big);
  *ost = rem_big;
  return res;
}

bool is_mantissa_overflow(s21_big_decimal* num) {
  return (num->bits[6] != 0 || num->bits[5] != 0 || num->bits[4] != 0 ||
          num->bits[3] != 0);
}
void reduce_scale(s21_big_decimal* num) {
  divide_by_10_with_banking_rounding(num);
  int scale = get_big_scale(*num);
  scale -= 1;
  set_big_scale(num, scale);
}
int normalize(s21_big_decimal* num) {
  while (is_mantissa_overflow(num) && get_big_scale(*num) > 0) {
    reduce_scale(num);
  }
  if (is_mantissa_overflow(num)) {
    return 0;
  }
  return 1;
}
int overflow_for_mul(s21_big_decimal value) {
  int res = 0;
  if (get_big_scale(value) > 28 && is_mantissa_overflow(&value)) {
    res = -1;
  } else if (get_big_scale(value) > 28 && !is_mantissa_overflow(&value)) {
    res = 1;
  } else if (is_mantissa_overflow(&value) && get_big_scale(value) < 28) {
    res = 2;
  }
  return res;
}
void result_for_mul(s21_big_decimal* big_result, int big_res_sign, int* res,
                    s21_decimal* result) {
  int result_scale = get_big_scale(*big_result);
  if (big_res_sign) {
    set_big_sign(big_result, big_res_sign);
  }
  int big_res_overflow = overflow_for_mul(*big_result);
  if (big_res_overflow == -1) {
    *res = big_res_sign == 1 ? 2 : 1;
  } else if (big_res_overflow == 1) {
    while (get_big_scale(*big_result) > 28 &&
           get_big_bit(*big_result, 224) == 0) {
      big_multiply_to_10(big_result, &result_scale);
    }
  } else if (big_res_overflow == 2) {
    normalize(big_result);
    *res = big_res_sign == 1 ? 2 : 1;
  }
  if (overflow_for_mul(*big_result) == 0) {
    big_to_decimal(result, *big_result);
    *res = 0;
  }
}
int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  int res = 0;
  int value_1_sign = get_sign(value_1);
  int value_2_sign = get_sign(value_2);
  if (value_1_sign == value_2_sign) {
    s21_big_decimal value_11 = {0};
    s21_big_decimal value_22 = {0};
    equal_scale_for_compare(value_1, value_2, &value_11, &value_22);
    int comparison_result = compare(value_11, value_22);

    if (value_1_sign == 0) {
      res = (comparison_result == -1) ? 1 : 0;
    } else {
      res = (comparison_result == 1) ? 1 : 0;
    }
  } else if (value_1_sign == 0 && value_2_sign == 1) {
    res = 0;
  } else if (value_1_sign == 1 && value_2_sign == 0) {
    res = 1;
  }
  return res;
}
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
  int res = 0;
  int value_1_sign = get_sign(value_1);
  int value_2_sign = get_sign(value_2);
  if (value_1_sign == value_2_sign) {
    s21_big_decimal value_11 = {0};
    s21_big_decimal value_22 = {0};
    equal_scale_for_compare(value_1, value_2, &value_11, &value_22);
    int comparison_result = compare(value_11, value_22);
    if (value_1_sign == 0) {
      res = (comparison_result == -1 || comparison_result == 0) ? 1 : 0;
    } else {
      res = (comparison_result == 1 || comparison_result == 0) ? 1 : 0;
    }
  } else if (value_1_sign == 0 && value_2_sign == 1) {
    res = 0;
  } else if (value_1_sign == 1 && value_2_sign == 0) {
    res = 1;
  }
  return res;
}
int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  int res = 0;
  int value_1_sign = get_sign(value_1);
  int value_2_sign = get_sign(value_2);
  if (value_1_sign == value_2_sign) {
    s21_big_decimal value_11 = {0};
    s21_big_decimal value_22 = {0};
    equal_scale_for_compare(value_1, value_2, &value_11, &value_22);
    int comparison_result = compare(value_11, value_22);

    if (value_1_sign == 0) {
      res = (comparison_result == 1) ? 1 : 0;
    } else {
      res = (comparison_result == -1) ? 1 : 0;
    }
  } else if (value_1_sign == 0 && value_2_sign == 1) {
    res = 1;
  } else if (value_1_sign == 1 && value_2_sign == 0) {
    res = 0;
  }
  return res;
}
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  int res = 0;
  int value_1_sign = get_sign(value_1);
  int value_2_sign = get_sign(value_2);
  if (value_1_sign == value_2_sign) {
    s21_big_decimal value_11 = {0};
    s21_big_decimal value_22 = {0};
    equal_scale_for_compare(value_1, value_2, &value_11, &value_22);
    int comparison_result = compare(value_11, value_22);

    if (value_1_sign == 0) {
      res = (comparison_result == 1 || comparison_result == 0) ? 1 : 0;
    } else {
      res = (comparison_result == -1 || comparison_result == 0) ? 1 : 0;
    }
  } else if (value_1_sign == 0 && value_2_sign == 1) {
    res = 1;
  } else if (value_1_sign == 1 && value_2_sign == 0) {
    res = 0;
  }
  return res;
}
int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  int res = 0;
  int value_1_sign = get_sign(value_1);
  int value_2_sign = get_sign(value_2);
  if (value_1_sign == value_2_sign) {
    s21_big_decimal value_11 = {0};
    s21_big_decimal value_22 = {0};
    equal_scale_for_compare(value_1, value_2, &value_11, &value_22);
    res = compare(value_11, value_22) == 0 ? 1 : 0;
  }
  return res;
}
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
  int res = 1;
  int value_1_sign = get_sign(value_1);
  int value_2_sign = get_sign(value_2);
  if (value_1_sign == value_2_sign) {
    s21_big_decimal value_11 = {0};
    s21_big_decimal value_22 = {0};
    equal_scale_for_compare(value_1, value_2, &value_11, &value_22);
    res = compare(value_11, value_22) != 0 ? 1 : 0;
  }
  return res;
}
int compare(s21_big_decimal value_1, s21_big_decimal value_2) {
  int res = 0;
  for (int i = 6; i >= 0; i--) {
    if (value_1.bits[i] > value_2.bits[i] && res == 0) {
      res = 1;
    } else if (value_1.bits[i] < value_2.bits[i] && res == 0) {
      res = -1;
    }
  }
  return res;
}
void equal_scale_for_compare(s21_decimal value_1, s21_decimal value_2,
                             s21_big_decimal* value_11,
                             s21_big_decimal* value_22) {
  decimal_to_big(value_1, value_11);
  decimal_to_big(value_2, value_22);
  if (get_scale(value_1) != get_scale(value_2)) {
    equal_big_scale(value_11, value_22);
  }
}
int s21_decimal_correct(s21_decimal value) {
  int res = 1;
  if (get_scale(value) < 0 || get_scale(value) > 28) {
    res = 0;
  }
  if ((value.bits[3] & 0x7F000000) != 0) {
    res = 0;
  }
  if ((value.bits[3] & 0x0000FFFF) != 0) {
    res = 0;
  }
  if (get_sign(value) != 0 && get_sign(value) != 1) {
    res = 0;
  }
  return res;
}

int s21_negate(s21_decimal value, s21_decimal* result) {
  s21_decl_to_null(result);
  s21_decimal copy = value;
  int sign = get_sign(value);
  if (sign) {
    set_sign(&copy, 0);
  } else {
    set_sign(&copy, 1);
  }
  *result = copy;

  return 0;
}

int s21_truncate(s21_decimal value, s21_decimal* result) {
  s21_decl_to_null(result);
  int error = 0;
  int scale = get_scale(value);
  int sign = get_sign(value);
  s21_big_decimal div = {0};
  s21_big_decimal ost = {0};
  decimal_to_big(value, &div);
  div.bits[7] = 0;

  if (scale > 28) {
    error = 1;
  } else {
    while (scale) {
      div = big_divide_by_10(&div, &ost);
      scale--;
    }
    set_big_sign(&div, sign);
    s21_from_big_to_dec(div, result);
  }
  return error;
}

int s21_floor(s21_decimal value, s21_decimal* result) {
  s21_decl_to_null(result);
  s21_decimal res = {0};
  s21_big_decimal ost = {0};
  s21_big_decimal nul = {0};
  s21_big_decimal one = {{1, 0, 0, 0, 0, 0, 0, 0}};
  int scale = get_scale(value);
  int sign = get_sign(value);
  value.bits[3] = 0;
  if (scale > 28) {
    return 1;
  }
  s21_big_decimal value_big = {0};
  decimal_to_big(value, &value_big);
  if (sign) {
    while (scale) {
      value_big = big_divide_by_10(&value_big, &ost);
      scale--;
    }
    if (compare(ost, nul) == 1) {
      s21_big_add(value_big, one, &value_big);
    }
    s21_from_big_to_dec(value_big, &res);
    set_sign(&res, sign);
    *result = res;
  } else {
    set_scale(&value, scale);
    s21_truncate(value, &res);
    *result = res;
  }

  return 0;
}

int s21_round(s21_decimal value, s21_decimal* result) {
  s21_decimal intPart;
  s21_decimal fracPart;
  s21_decimal val;
  s21_decimal zero_val = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}};
  s21_decimal half_val = {{0x00000005, 0x00000000, 0x00000000, 0x00010000}};
  s21_decimal one_val = {{0x00000001, 0x00000000, 0x00000000, 0x00000000}};
  int negative_check = 0;
  int error = 0;
  if (get_scale(value) <= 28) {
    val = value;
    if (s21_is_less(val, zero_val)) {
      s21_negate(val, &val);
      negative_check = 1;
    }
    s21_truncate(val, &intPart);
    s21_sub(val, intPart, &fracPart);
    if (!s21_is_less(fracPart, half_val))
      s21_add(intPart, one_val, &val);
    else
      val = intPart;
    if (negative_check == 1) s21_negate(val, &val);
    *result = val;
  } else {
    error = 1;
  }
  return error;
}

void s21_decl_to_null(s21_decimal* decl) {
  for (int i = 0; i < 4; i++) {
    decl->bits[i] = 0;
  }
}

void s21_big_decl_to_null(s21_big_decimal* decl) {
  for (int i = 0; i < 8; i++) {
    decl->bits[i] = 0;
  }
}

int s21_from_int_to_decimal(int src, s21_decimal* dst) {
  int sign = 0;

  s21_decl_to_null(dst);

  if (src < 0) {
    sign = 1;
    src = -src;
  }

  dst->bits[0] = src;

  set_scale(dst, 0);

  set_sign(dst, sign);

  return 0;
}

int s21_from_float_to_decimal(float src, s21_decimal* dst) {
  int res = 0;
  s21_decl_to_null(dst);
  long double max_value = powl(2.0, 96);
  if (isnan(src)) {
    res = 1;
  } else if (fabs(src) != 0 && fabs(src) < powl(10.0, -1 * 28)) {
    res = 1;

  } else if (src >= max_value) {
    res = 1;

  } else if (src <= -1 * max_value) {
    res = 1;

  } else if (src != 0) {
    int scale = 0;
    int negative = 0;
    if (src < 0.0) {
      negative = 1;
    }

    src = fabsl(src);

    char buffer[50];
    char buffer2[50];
    int i = 0;

    sprintf(buffer, "%.8g", src);
    char* dot = strchr(buffer, '.');
    *dot = '\0';
    dot++;
    while (*dot != '\0') {
      buffer2[i] = *dot;
      i++;
      dot++;
    }
    buffer2[i] = '\0';
    strcat(buffer, buffer2);
    int mantissa = atoi(buffer);
    s21_from_int_to_decimal(mantissa, dst);

    if (negative) {
      set_sign(dst, 1);
    }
    scale = strlen(buffer2);
    set_scale(dst, scale);
  }

  return res;
}

int s21_from_decimal_to_int(s21_decimal src, int* dst) {
  int failed = 0;
  int scale = get_scale(src);
  if (dst) {
    for (int i = 0; i < scale; i++) {
      remove_last_digit(&src);
    }

    if (src.bits[1] == 0 && src.bits[2] == 0 &&
        (src.bits[0] <= INT_MAX ||
         (src.bits[0] == (unsigned int)INT_MIN && get_sign(src)))) {
      *dst = (get_sign(src)) ? (-1) : 1;
      *dst *= src.bits[0];
    } else {
      failed = 1;
    }
  } else {
    failed = 1;
  }

  return failed;
}

void remove_last_digit(s21_decimal* num) {
  unsigned long remainder = 0;
  for (int i = 2; i >= 0; i--) {
    unsigned long value = (remainder << 32) + num->bits[i];
    num->bits[i] = (value / 10);
    remainder = value % 10;
  }
}

int s21_from_decimal_to_float(s21_decimal src, float* dst) {
  int res = 0;
  if (dst) {
    *dst = 0.0;
    int scale = get_scale(src);
    for (int i = 0; i < 96; i++) {
      if (get_bit(src, i)) {
        *dst += powf(2.0f, i);
      }
    }
    *dst /= powf(10.0, scale);
    if (get_sign(src)) {
      *dst = *dst * (-1);
    }
  } else {
    res = 1;
  }
  return res;
}