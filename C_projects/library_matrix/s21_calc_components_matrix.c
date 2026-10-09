#include "s21_matrix.h"

int s21_calc_complements(matrix_t* A, matrix_t* result) {
  if (A == NULL || result == NULL) {
    return INCORRECT_MATRIX;
  }

  if (A->rows != A->columns) {
    return CALCULATION_ERR;
  }

  if (A->rows == 0 || A->columns == 0) {
    return INCORRECT_MATRIX;
  }

  int n = A->rows;
  s21_create_matrix(n, n, result);

  matrix_t minor;
  s21_create_matrix(n - 1, n - 1, &minor);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      s21_get_minor(A, &minor, i, j);

      double det = 0.0;
      s21_determinant(&minor, &det);

      result->matrix[i][j] = det * ((i + j) % 2 == 0 ? 1 : -1);
    }
  }

  s21_remove_matrix(&minor);

  return OK;
}