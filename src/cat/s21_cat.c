#include "s21_cat.h"

int main(int argc, char **argv) {
  Flags flags = CatReadFlags(argc, argv);
  if (flags.illegalOption == 0) {
    for (int i = optind; i < argc; i++) {
      FILE *f = fopen(argv[i], "r");
      if (f != NULL) {
        printCatWithFlags(f, flags);
      } else {
        fprintf(stderr, "No such file or directory");
      }
    }
  } else {
    fprintf(stderr, "cat: invalid option");
  }
  return 0;
}

Flags CatReadFlags(int argc, char *argv[]) {
  struct option longOptions[] = {{"number-nonblank", 0, NULL, 'b'},
                                 {"number", 0, NULL, 'n'},
                                 {"squeeze-blank", 0, NULL, 's'},
                                 {NULL, 0, NULL, 0}};
  Flags flags = {false, false, false, false, false, false, 0, 0};
  const char *short_options = "+bestnvTE";
  char currentFlag;
  while ((currentFlag = getopt_long(argc, argv, short_options, longOptions,
                                    NULL)) != -1) {
    if (currentFlag == 'b') {
      flags.numberNonBlank = true;
      flags.counter++;
    }
    if (currentFlag == 'e') {
      flags.markEndl = true;
      flags.printNonPrintable = true;
      flags.counter++;
    }
    if (currentFlag == 'v') {
      flags.printNonPrintable = true;
      flags.counter++;
    }
    if (currentFlag == 'n') {
      flags.numberAll = true;
      flags.counter++;
    }
    if (currentFlag == 'E') {
      flags.markEndl = true;
      flags.counter++;
    }
    if (currentFlag == 's') {
      flags.squeeze = true;
      flags.counter++;
    }
    if (currentFlag == 't') {
      flags.tab = true;
      flags.printNonPrintable = true;
      flags.counter++;
    }
    if (currentFlag == 'T') {
      flags.counter++;
      flags.tab = true;
    }
    if (flags.counter == 0) flags.illegalOption = 1;
  }
  return flags;
}

void printCatWithFlags(FILE *f, Flags flags) {
  char carentchar;
  int countSlash = 1, countStart = 0, countString = 1;
  while ((carentchar = fgetc(f)) != EOF) {
    // if (flags.counter == 0) printf("%c", carentchar);  // zero flags
    if (flags.squeeze) {  // -s start
      if (carentchar != '\n') countSlash = 0;
      if (carentchar == '\n' && countSlash < 3) countSlash++;
      if (carentchar == '\n' && countSlash > 2) continue;
    }  // -s stop / -n start
    if (flags.numberAll && (flags.numberNonBlank == 0)) {
      if (countStart == 0) {
        printf("%6d\t", countString);
        countString++;
        countStart = 1;
      }
      if (carentchar == '\n') countStart = 0;
    }  // -b start
    if (flags.numberNonBlank) {
      if (countStart == 0 && carentchar != '\n') {
        printf("%6d\t", countString);
        countString++;
        countStart = 1;
      }
      if (carentchar == '\n') countStart = 0;
    }  // -b stop / -e/E start and t/T
    if (flags.markEndl && carentchar == '\n') printf("$");
    if (flags.printNonPrintable) {  // -v
      if (carentchar >= 0 && carentchar != 10 && carentchar != 9 &&
          carentchar < 32) {
        printf("^");
        carentchar += 64;
      }
      if (carentchar == 127) {
        printf("^?");
      } else {
        if (flags.tab && (carentchar == 9)) {
          printf("^I");
        } else {
          printf("%c", carentchar);
        }
      }
    } else {
      printf("%c", carentchar);
    }
  }
}