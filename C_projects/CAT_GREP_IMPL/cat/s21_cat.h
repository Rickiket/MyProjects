#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#define _GNU_SOURCE

#include <getopt.h>
#include <stdio.h>

typedef struct argum {
  int b, n, s, E, T, e, t, v, er;
} sargum;

sargum argumets(int argc, char **argv);
void out(int c, sargum argg, int *prosh, int *num, int *pis);

#endif  // SRC_CAT_S21_CAT_H_
