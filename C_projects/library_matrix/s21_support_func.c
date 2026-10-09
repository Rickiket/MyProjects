#include "s21_matrix.h"
int negative_size(int rows, int columns) { return (rows <= 0 || columns <= 0); }
int eq_size(matrix_t* A, matrix_t* B) {
  return ((A->rows != B->rows) || (A->columns != B->columns));
}
int nan_value(matrix_t* A) {
  int res = OK;
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      if (isinf(A->matrix[i][j]) || isnan(A->matrix[i][j])) {
        res = CALCULATION_ERR;
        i = A->rows;
        j = A->columns;
      }
    }
  }
  return res;
}
void s21_get_minor(matrix_t* A, matrix_t* minor, int row, int col) {
  int minor_row = 0, minor_col = 0;

  for (int i = 0; i < A->rows; i++) {
    if (i == row) continue;
    minor_col = 0;
    for (int j = 0; j < A->columns; j++) {
      if (j == col) continue;
      minor->matrix[minor_row][minor_col] = A->matrix[i][j];
      minor_col++;
    }
    minor_row++;
  }
}
void s21_initialize_matrix(matrix_t* A, double start_value,
                           double iteration_step) {
  if (A != NULL && A->matrix != NULL) {
    double value = start_value;
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        A->matrix[i][j] = value;
        value += iteration_step;
      }
    }
  }
}