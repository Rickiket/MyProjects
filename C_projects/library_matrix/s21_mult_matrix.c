#include "s21_matrix.h"
int s21_mult_number(matrix_t* A, double number, matrix_t* result) {
  if (A == NULL || result == NULL || A->matrix == NULL) {
    return INCORRECT_MATRIX;
  }
  if (isinf(number) || isnan(number) || nan_value(A) != 0) {
    return CALCULATION_ERR;
  }
  int res = OK;
  if (s21_create_matrix(A->rows, A->columns, result) == 0) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = number * A->matrix[i][j];
      }
    }
  } else {
    res = INCORRECT_MATRIX;
  }
  return res;
}
int s21_mult_matrix(matrix_t* A, matrix_t* B, matrix_t* result) {
  if (A == NULL || B == NULL || result == NULL) {
    return INCORRECT_MATRIX;
  }
  if (A->columns != B->rows || nan_value(A) != 0 || nan_value(B) != 0) {
    return CALCULATION_ERR;
  }
  int res = OK;
  if (s21_create_matrix(A->rows, B->columns, result) == 0) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < B->columns; j++) {
        for (int k = 0; k < A->columns; k++) {
          result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
        }
      }
    }

  } else {
    res = CALCULATION_ERR;
  }
  return res;
}