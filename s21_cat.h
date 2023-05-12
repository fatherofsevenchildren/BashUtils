#ifndef CAT_H
#define CAT_H

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct Flags {
  bool numberNonBlank;
  bool markEndl;
  bool numberAll;
  bool squeeze;
  bool tab;
  bool printNonPrintable;
  int counter;
  int illegalOption;
} Flags;

int getopt_long(int argc, char *const argv[], const char *optstring,
                const struct option *longopts, int *longindex);

Flags CatReadFlags(int argc, char *argv[]);

void printCatWithFlags(FILE *f, Flags flags);

#endif