#include "s21_matrix.h"
void s21_swap_rows(matrix_t* A, int row1, int row2) {
  for (int j = 0; j < A->columns; j++) {
    double temp = A->matrix[row1][j];
    A->matrix[row1][j] = A->matrix[row2][j];
    A->matrix[row2][j] = temp;
  }
}
int s21_determinant(matrix_t* A, double* result) {
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
  double det = 1.0;
  matrix_t copy;
  s21_create_matrix(n, n, &copy);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      copy.matrix[i][j] = A->matrix[i][j];
    }
  }
  for (int i = 0; i < n; i++) {
    int max_row = i;
    for (int k = i + 1; k < n; k++) {
      if (fabs(copy.matrix[k][i]) > fabs(copy.matrix[max_row][i])) {
        max_row = k;
      }
    }
    if (fabs(copy.matrix[max_row][i]) < 1e-12) {
      det = 0.0;
      break;
    }
    if (max_row != i) {
      s21_swap_rows(&copy, i, max_row);
      det *= -1;
    }
    for (int k = i + 1; k < n; k++) {
      double factor = copy.matrix[k][i] / copy.matrix[i][i];
      for (int j = i; j < n; j++) {
        copy.matrix[k][j] -= factor * copy.matrix[i][j];
      }
    }
  }
  for (int i = 0; i < n; i++) {
    det *= copy.matrix[i][i];
  }
  s21_remove_matrix(&copy);
  *result = det;
  return OK;
}