#include "s21_matrix.h"
int s21_sum_matrix(matrix_t* A, matrix_t* B, matrix_t* result) {
  if (A == NULL || B == NULL) {
    return INCORRECT_MATRIX;
  } else if (eq_size(A, B) || nan_value(A) || nan_value(B)) {
    return CALCULATION_ERR;
  }
  int res = OK;
  if (s21_create_matrix(A->rows, A->columns, result) == 0) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
      }
    }
  } else {
    res = INCORRECT_MATRIX;
  }
  return res;
}
int s21_sub_matrix(matrix_t* A, matrix_t* B, matrix_t* result) {
  if (A == NULL || B == NULL) {
    return INCORRECT_MATRIX;
  } else if (eq_size(A, B) || nan_value(A) || nan_value(B)) {
    return CALCULATION_ERR;
  }
  int res = OK;
  if (s21_create_matrix(A->rows, A->columns, result) == 0) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
      }
    }
  } else {
    res = INCORRECT_MATRIX;
  }
  return res;
}