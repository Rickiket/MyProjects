#include "s21_matrix.h"
int s21_transpose(matrix_t* A, matrix_t* result) {
  if (A == NULL || result == NULL) {
    return INCORRECT_MATRIX;
  }
  if (A->rows <= 0 || A->columns <= 0) {
    return INCORRECT_MATRIX;
  }
  if (s21_create_matrix(A->columns, A->rows, result) == 0) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[j][i] = A->matrix[i][j];
      }
    }
  }
  return OK;
}