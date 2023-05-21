#include "s21_grep.h"

int main(int argc, char **argv) {
  char pattern[4000] = {0};
  Flags flags = read_flags(argc, argv, pattern);
  if (flags.illigal == 0) {
    int countTxt = 0;
    for (int i = optind; i < argc; i++) {  // для подсчета текстовых файлов
      FILE *f = fopen(argv[i], "r");
      if (f != NULL) {
        countTxt++;
        fclose(f);
      }
    }
    int i = optind + 1;
    if ((flags.counter > 1 && flags.e) || flags.e || flags.f) i--;
    for (; i < argc; i++) {
      FILE *f = fopen(argv[i], "r");
      if (f != NULL) {
        print_grep(f, flags, argv, i, countTxt, pattern);
        fclose(f);
      } else {
        if (!flags.s)
          fprintf(stderr, "grep: %s: No such file or directory\n", argv[i]);
      }
    }
  } else {
    if (!flags.s) fprintf(stderr, "zachem ti eto vvel");
  }
  return 0;
}

Flags read_flags(int argc, char *argv[], char *pattern) {
  struct option longOptions[] = {{"number-nonblank", 0, NULL, 'b'},
                                 {NULL, 0, NULL, 0}};
  Flags flags = {0,     false, false, false, false, false, false,
                 false, 0,     false, 0,     0,     0};
  const char *short_options = "e:ivclnhsf:o";
  char currentFlag;
  while ((currentFlag = getopt_long(argc, argv, short_options, longOptions,
                                    NULL)) != -1) {
    parsing_flags(&flags, currentFlag, pattern);
  }
  return flags;
}

/// @param i счетчик текущего файла для печати названия файла в флаг -l
/// @param countTxt кол-во текстовых файловç

void print_grep(FILE *f, Flags flags, char *argv[], int i, int countTxt,
                char *pattern) {
  if (flags.e == 0 && flags.f == 0)
    pattern = argv[optind];  // это точно работает тут лежит патерн я проверил
  regex_t regex;
  int cflags = REG_EXTENDED;  // флаг для выпоолнения
  if (flags.i) cflags = REG_ICASE;  // reg_icase не учитывает регистр
  int resultComp = regcomp(
      &regex, pattern, cflags);  /// для компиляции паттерна (возвращает 0 если
                                 /// все окей) // работает ништяк я все проверил
                                 /// все окей) // работает ништяк я все проверил
  if (resultComp == 0) {  // проверка что регкомп сработал
    print_flags(f, regex, flags, countTxt, argv[i]);
  }
}

void print_flags(FILE *f, regex_t regex, Flags flags, int countTxt,
                 char *argv) {
  regmatch_t match_reg[1];  // для regex
  int countStringsMatch = 0, countStringAll = 0,
      flagMatch = 0;  // для флага -с // для флага -n // для флага -l
  while (!feof(f)) {
    countStringAll++;
    char text[4096] = {0};
    fgets(text, 4096, f);
    int result = regexec(&regex, text, 1, match_reg,
                         0);  // если были совпадения то вернет 0
    if (strchr(text, '\n') == NULL) strcat(text, "\n");  //
    if (result == 0) {
      if (countTxt > 1 && !flags.v && !flags.l && !flags.c && !flags.h)
        printf("%s:", argv);
      if (flags.n && !flags.v) printf("%d:", countStringAll);
      if (!flags.c && !flags.l && !flags.v && !flags.o) printf("%s", text);
      countStringsMatch++;
      flagMatch++;
    }
    if (flags.o && !flags.v) {
      char *charPattern = text;
      while (!result && match_reg[0].rm_eo != match_reg[0].rm_so) {
        printf("%.*s\n", (int)(match_reg[0].rm_eo - match_reg[0].rm_so),
               charPattern + match_reg[0].rm_so);
        charPattern += match_reg[0].rm_eo;
        result = regexec(&regex, charPattern, 1, match_reg, 0);
      }
    }
    if (flags.v && result) flagMatch++;
    if (flags.v && !flags.c && result == 1 && !flags.l) {
      if (countTxt > 1 && !flags.h) {
        flags.n ? printf("%s:%d:%s", argv, countStringAll, text)
                : printf("%s:%s", argv, text);
      } else {
        flags.n ? printf("%d:%s", countStringAll, text) : printf("%s", text);
      }
    }
  }
  if (flags.c) {
    print_c(flags, argv, countStringAll, countStringsMatch, countTxt);
  }
  if (flags.l) {
    if (flagMatch > 0 || flags.v) printf("%s\n", argv);
  }
  regfree(&regex);
}

void print_c(Flags flags, char *argv, int countStringAll, int countStringsMatch,
             int countTxt) {
  if (flags.v && !flags.l) {
    if (countTxt > 1 && !flags.h) {
      printf("%s:%d\n", argv, countStringAll - countStringsMatch);
    } else {
      printf("%d\n", countStringAll - countStringsMatch);  // для флага -vc
    }
  } else if (flags.l) {
    if (countTxt > 1 && !flags.h) {
      printf("%s:%d\n", argv, countStringsMatch > 0 ? 1 : 0);
    } else {
      printf("%d\n", countStringsMatch > 0 ? 1 : 0);
    }
  } else {
    if (countTxt > 1 && !flags.h) {
      printf("%s:%d\n", argv, countStringsMatch);
    } else {
      printf("%d\n", countStringsMatch);
    }
  }
}

void parsing_flags(Flags *flags, int currentFlag, char *pattern) {
  if (currentFlag == 'e') {
    if (flags->FirstE != 0) strcat(pattern, "|");
    strcat(pattern, optarg);
    flags->FirstE = 1;
    flags->e++;
    flags->counter++;
  }
  if (currentFlag == 'i') {
    flags->i = true;
    flags->counter++;
  }
  if (currentFlag == 'v') {
    flags->v = true;
    flags->counter++;
  }
  if (currentFlag == 'c') {
    flags->c = true;
    flags->n = false;
    flags->counter++;
  }
  if (currentFlag == 'l') {
    flags->l = true;
    flags->h = false;
    flags->n = false;
    flags->counter++;
  }
  if (currentFlag == 'n') {
    if (!flags->c && !flags->l) flags->n = true;
    flags->counter++;
  }
  if (currentFlag == 'h') {
    if (!flags->l) flags->h = true;
    flags->counter++;
  }
  if (currentFlag == 's') {
    flags->s = true;
    flags->counter++;
  }
  if (currentFlag == 'f') {
    read_fflag(flags, pattern);
  }
  if (currentFlag == 'o') {
    flags->o = true;
    flags->counter++;
  }
}

void read_fflag(Flags *flags, char *pattern) {
  char *file = optarg;
  FILE *fp;
  fp = fopen(file, "r");
  if (fp != NULL) {
    int i = 0;
    int flagFirstF = 1;
    int currentCym;
    while ((currentCym = fgetc(fp)) != EOF && i < 4000) {
      if (flagFirstF == 1) {
        pattern[i] = currentCym;
        flagFirstF = 0;
      }
      if (currentCym == '\n' || currentCym == 13) {
        currentCym = '|';
        pattern[i++] = currentCym;
      } else {
        pattern[i++] = currentCym;
      }
    }
    fclose(fp);
  } else {
    fprintf(stderr, "zachem");
  }
  flags->f++;
  flags->counter++;
}
