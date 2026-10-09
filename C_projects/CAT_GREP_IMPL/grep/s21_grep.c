#include "s21_grep.h"

int main(int argc, char **argv) {
  Flags arg = arguments(argc, argv);
  int flag_ptint = 0;
  while (optind < argc) {
    if (argc - optind > 1) {
      flag_ptint = 1;
    }
    output(argv, arg, &flag_ptint);
    optind++;
  }
  return 0;
}

Flags arguments(int argc, char **argv) {
  int opt = 0;
  Flags arg = {0};
  while ((opt = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, 0)) != -1) {
    switch (opt) {
      case 'e':
        arg.e = 1;
        strcat(arg.str_reg, optarg);
        strcat(arg.str_reg, "|");
        break;
      case 'i':
        arg.i = 1;
        break;
      case 'v':
        arg.v = 1;
        arg.o = 0;
        break;
      case 'n':
        if (arg.c != 1 && arg.l != 1) arg.n = 1;
        break;
      case 'c':
        arg.c = 1;
        arg.n = 0;
        break;
      case 'l':
        arg.l = 1;
        arg.n = 0;
        break;
      case 'h':
        arg.h = 1;
        break;
      case 's':
        arg.s = 1;
        break;
      case 'f':
        arg.f = 1;
        arg.s = 0;
        f_flag(optarg, &arg);
        break;
      case 'o':
        if (arg.v != 1) arg.o = 1;
        break;
      default:
        break;
    }
    if (arg.len != 0) {
      optind++;
    }
  }
  flag_e(argc, argv, &arg);
  return arg;
}

void flag_e(int argc, char **argv, Flags *arg) {
  if (!arg->e && !arg->f) {
    if (argc > optind) {
      strcat(arg->str_reg, argv[optind]);
    }
    optind++;
  }

  if (arg->e || arg->f) {
    arg->str_reg[strlen(arg->str_reg) - 1] = '\0';
  }
}

void output(char **argv, Flags arg, int *flag_ptint) {
  FILE *file;
  regex_t reg;
  regmatch_t start = {0};
  int str_count = 0;
  int str_number = 0;
  int flag_c = 0;
  int flag_i = REG_EXTENDED;
  if (arg.i) {
    flag_i = REG_EXTENDED | REG_ICASE;
  }
  file = fopen(argv[optind], "r");
  if (file == NULL && !(arg.s)) {
    fprintf(stderr, "grep: %s: No such file or directory\n", argv[optind]);
    return;
  }
  regcomp(&reg, arg.str_reg, flag_i);
  if (file != NULL) {
    while (fgets(arg.str, BUFFER, file) != NULL) {
      int match = regexec(&reg, arg.str, 1, &start, 0);

      if (!match) flag_c = 1;
      if (arg.v) match = !match;
      str_number++;
      if (!match) str_count++;
      if (!match && *flag_ptint && !arg.c && !arg.l && !arg.h)
        printf("%s:", argv[optind]);
      if (!match && arg.n) printf("%d:", str_number);
      if (!match && !arg.c && !arg.l && !arg.o) {
        printf("%s", arg.str);
        if (strchr(arg.str, '\n') == NULL) {
          printf("\n");
        }
      }
      if (arg.o && !arg.l && !arg.c) print_matches(arg, &start, &reg);
    }
    flag_cl(arg, argv, flag_ptint, &flag_c, &str_count);
    fclose(file);
  }
  regfree(&reg);
}

void flag_cl(Flags arg, char *argv[optind], int *flag_ptint, int *flag_c,
             int *str_count) {
  if (arg.c && !arg.l) {
    if (arg.h == 0) {
      if (*flag_ptint) printf("%s:", argv[optind]);
    }
    printf("%d\n", *str_count);
  }

  if (arg.l && !arg.c && *flag_c == 1) printf("%s\n", argv[optind]);

  if (arg.c && arg.l) {
    if (*flag_ptint && *flag_c == 1) {
      printf("%s:%d\n", argv[optind], *flag_c);
      printf("%s\n", argv[optind]);
    }
    if (!(*flag_ptint) && *flag_c == 1)
      printf("%d\n%s\n", *flag_c, argv[optind]);
    if (!(*flag_ptint) && *flag_c == 0) printf("%d\n", *flag_c);
    if ((*flag_ptint) && *flag_c == 0) printf("%s:%d\n", argv[optind], *flag_c);
  }
}

void f_flag(char *filename, Flags *arg) {
  FILE *f;
  f = fopen(filename, "r");
  if (f != NULL) {
    while (!feof(f)) {
      if (fgets(arg->str, 1000, f) != NULL) {
        if (arg->str[strlen(arg->str) - 1] == '\n' && strlen(arg->str) - 1 != 0)
          arg->str[strlen(arg->str) - 1] = '\0';
        strcat(arg->str_reg, arg->str);
        strcat(arg->str_reg, "|");
      }
    }
    fclose(f);
  } else if (!arg->s) {
    fprintf(stderr, "grep: %s: No such file or directory\n", filename);
  }
}

void print_matches(Flags arg, regmatch_t *start, regex_t *reg) {
  char *cursor = arg.str;
  while (!regexec(reg, cursor, 1, start, 0)) {
    printf("%.*s\n", (int)(start->rm_eo - start->rm_so), cursor + start->rm_so);
    cursor += start->rm_eo;
  }
}