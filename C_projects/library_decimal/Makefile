CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c11

OS=$(shell uname -s)

FILES= s21_decimal.c
OBJECTS=$(FILES:.c=.o)

ifeq ($(OS), Linux)
	TEST_LIBS = -pthread -lcheck_pic -pthread -lrt -lm -lsubunit --coverage
endif

ifeq ($(OS), Darwin)
	TEST_LIBS = -lcheck
endif

all: s21_decimal.a

rebuild: clean all

s21_decimal.a: $(OBJECTS)
	ar rc s21_decimal.a $(OBJECTS)
	ranlib s21_decimal.a

test: test.c s21_decimal.a
	$(CC) $(CFLAGS) -o tests test.c s21_decimal.a $(TEST_LIBS) -lm
	./tests

gcov_report: $(FILES) s21_decimal.a
	$(CC) $(CFLAGS) --coverage -c $(FILES)
	$(CC) $(CFLAGS) --coverage -o tests $(OBJECTS) test.c s21_decimal.a $(TEST_LIBS)
	./tests
	gcov $(FILES)
	lcov -t "stest" -o s21_test.info -c -d .
	genhtml -o report s21_test.info
	open report/index.html

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o s21_decimal.a tests *.gcda *.gcno *.gcov s21_test.info report
	
valgrind:
	CK_FORK=no valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all --verbose --log-file=RESULT_VALGRIND.txt ./tests
