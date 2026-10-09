#include "s21_cat.h"

#include <getopt.h>
#include <stdio.h>

sargum argumets(int argc, char **argv) {
  sargum arg = {0};
  int opt;
  struct option longopt[] = {{"number-nonblank", no_argument, NULL, 'b'},
                             {"number", no_argument, NULL, 'n'},
                             {"squeeze-blank", no_argument, NULL, 's'},
                             {0, 0, 0, 0}};
  while ((opt = getopt_long(argc, argv, "beEnstTv", longopt, 0)) != -1) {
    switch (opt) {
      case 'b':
        arg.b = 1;
        break;
      case 'e':
        arg.E = 1;
        arg.v = 1;
        break;
      case 'E':
        arg.E = 1;
        break;
      case 'n':
        arg.n = 1;
        break;
      case 's':
        arg.s = 1;
        break;
      case 't':
        arg.T = 1;
        arg.v = 1;
        break;
      case 'T':
        arg.T = 1;
        break;
      case 'v':
        arg.v = 1;
        break;
      case '?':
        arg.er = 1;
        break;
      default:
        arg.er = 1;
        break;
    }
  }
  return arg;
}

void out(int c, sargum argg, int *prosh, int *num, int *pis) {
  if (!(argg.s && *prosh == '\n' && c == '\n' && *pis)) {
    if (*prosh == '\n' && c == '\n') {
      *pis = 1;
    } else {
      *pis = 0;
    }
    if ((argg.n && argg.b == 0) && *prosh == '\n') {
      printf("%6d\t", *num);
      *num = *num + 1;
    } else if ((argg.b && c != '\n') && *prosh == '\n') {
      printf("%6d\t", *num);
      *num = *num + 1;
    }
    if (argg.E) {
      if (c == '\n') {
        printf("$");
      }
    }
    if (argg.T && c == 9) {
      printf("^");
      c = 'I';
    }
    if (argg.v) {
      if (c > 127 && c < 160) printf("M-^");
      if ((c < 32 && c != '\n' && c != '\t') || c == 127) printf("^");
      if ((c < 32 || (c > 126 && c < 160)) && c != '\n' && c != '\t')
        c = c > 126 ? c - 128 + 64 : c + 64;
    }
    fputc(c, stdout);
  }
  *prosh = c;
}

int main(int argc, char **argv) {
  sargum argg = argumets(argc, argv);
  if (argg.er) {
    printf("Error");
  } else {
    for (int i = 1; i < argc; i++) {
      if (argv[i][0] != '-') {
        FILE *f = fopen(argv[i], "r");
        if (f != NULL) {
          int prosh = '\n';
          int num = 1;
          int pis = 0;
          int c = fgetc(f);
          while (c != EOF) {
            out(c, argg, &prosh, &num, &pis);
            c = fgetc(f);
          }
        } else {
          printf("No such file: %s\n", argv[i]);
        }
      }
    }
  }
  return 0;
}
