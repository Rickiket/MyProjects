#include "s21_matrix.h"
int s21_eq_matrix(matrix_t* A, matrix_t* B) {
  if (A == NULL || B == NULL || eq_size(A, B)) {
    return FAILURE;
  }
  int res = SUCCESS;
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      if (fabs(A->matrix[i][j] - B->matrix[i][j]) > 1e-7) {
        res = FAILURE;
        i = A->rows;
        j = A->columns;
      }
    }
  }
  return res;
}
