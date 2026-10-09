#include "s21_matrix.h"
int s21_create_matrix(int rows, int columns, matrix_t* result) {
  int res = OK;
  if (negative_size(rows, columns) || result == NULL) {
    return INCORRECT_MATRIX;
  }
  result->matrix = (double**)calloc(rows, sizeof(double*));
  if (result->matrix == NULL) {
    res = INCORRECT_MATRIX;
  } else {
    for (int i = 0; i < rows; i++) {
      result->matrix[i] = (double*)calloc(columns, sizeof(double));
    }
    if (res == OK) {
      result->rows = rows;
      result->columns = columns;
    }
  }
  return res;
}