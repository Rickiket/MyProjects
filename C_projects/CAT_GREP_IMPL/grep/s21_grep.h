#ifndef S21_GREP_H
#define S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER 10000

typedef struct {
  int e, i, v, c, l, n, h, s, f, o, len;
  char str[BUFFER];
  char str_reg[BUFFER];

} Flags;

void print_matches(Flags arg, regmatch_t *start, regex_t *reg);
void f_flag(char *filename, Flags *arg);
Flags arguments(int argc, char **argv);
void output(char *argv[], Flags arg, int *flag_ptint);
void flag_cl(Flags arg, char *argv[optind], int *flag_ptint, int *flag_c,
             int *str_count);
void flag_e(int argc, char **argv, Flags *arg);

#endif