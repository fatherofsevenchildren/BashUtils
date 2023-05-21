C = gcc -std=c11
FLAGS = -Wall -Werror -Wextra -g

all: s21_grep

s21_grep: 
	$(CC) $(FLAGS) s21_grep.c -o s21_grep


rebuild: clean s21_grep
	

clean:
	rm -rf s21_grep
