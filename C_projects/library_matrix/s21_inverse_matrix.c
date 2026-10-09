#include "s21_matrix.h"
int s21_inverse_matrix(matrix_t* A, matrix_t* result) {
  if (A == NULL || result == NULL) {
    return INCORRECT_MATRIX;
  } else if (A->rows != A->columns || nan_value(A)) {
    return CALCULATION_ERR;
  }
  if (A->rows == 0 || A->columns == 0) {
    return INCORRECT_MATRIX;
  }
  int res = OK;
  double det = 0.;
  s21_determinant(A, &det);

  if (det) {
    matrix_t complements, transpone;
    s21_calc_complements(A, &complements);
    s21_transpose(&complements, &transpone);
    det = 1.0 / det;
    s21_mult_number(&transpone, det, result);
    s21_remove_matrix(&complements);
    s21_remove_matrix(&transpone);
  } else {
    res = CALCULATION_ERR;
  }

  return res;
}