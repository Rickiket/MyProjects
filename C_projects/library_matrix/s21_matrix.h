#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define SUCCESS 1
#define FAILURE 0
typedef struct matrix_struct {
  double** matrix;
  int rows;
  int columns;
} matrix_t;
enum work_res { OK, INCORRECT_MATRIX, CALCULATION_ERR };
int s21_create_matrix(int rows, int columns, matrix_t* result);
void s21_remove_matrix(matrix_t* A);
int s21_eq_matrix(matrix_t* A, matrix_t* B);
int s21_sum_matrix(matrix_t* A, matrix_t* B, matrix_t* result);
int s21_sub_matrix(matrix_t* A, matrix_t* B, matrix_t* result);
int s21_mult_number(matrix_t* A, double number, matrix_t* result);
int s21_mult_matrix(matrix_t* A, matrix_t* B, matrix_t* result);
int s21_transpose(matrix_t* A, matrix_t* result);
int s21_calc_complements(matrix_t* A, matrix_t* result);
int s21_determinant(matrix_t* A, double* result);
int s21_inverse_matrix(matrix_t* A, matrix_t* result);

int negative_size(int rows, int columns);
int eq_size(matrix_t* A, matrix_t* B);
int nan_value(matrix_t* A);
void s21_get_minor(matrix_t* A, matrix_t* minor, int row, int col);

#ifndef S21_HELPER_FUNCTIONS
#define S21_HELPER_FUNCTIONS

void s21_initialize_matrix(matrix_t* A, double start_value,
                           double iteration_step);

#endif  // S21_HELPER_FUNCTIONS
