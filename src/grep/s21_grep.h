#ifndef CAT_H
#define CAT_H

#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct Flags {
  int e;
  bool i;
  bool v;
  bool c;
  bool l;
  bool n;
  bool h;
  bool s;
  int f;
  bool o;
  int counter;
  int illigal;
  int FirstE;
} Flags;

void print_grep(FILE *f, Flags flags, char *argv[], int i, int countTxt,
                char *pattern);
void parsing_flags(Flags *flags, int currentFlag, char *pattern);
void print_c(Flags flags, char *argv, int countStringAll, int countStringsMatch,
             int countTxt);
void print_flags(FILE *f, regex_t regex, Flags flags, int countTxt, char *argv);
void read_fflag(Flags *flags, char *pattern);

void print_grep(FILE *f, Flags flags, char *argv[], int i, int countTxt,
                char *pattern);

int getopt_long(int argc, char *const argv[], const char *optstring,
                const struct option *longopts, int *longindex);

Flags read_flags(int argc, char *argv[], char *pattern);

#endif