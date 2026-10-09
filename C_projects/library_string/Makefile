SRC=$(wildcard s21_*.c)
OBJ=$(patsubst %.c,%.o,$(SRC))
BUILD_FLAGS=-std=c11 -Wall -Werror -Wextra
TEST_FLAGS=-lcheck
LINUX_FLAGS=-lpthread -pthread -lrt -lsubunit -lm
VALGRIND_FLAGS=--trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all --verbose
EXE_NAME= s21_test
CC=gcc

all: s21_string.a

s21_string.a: $(OBJ)
	ar	rcs	$@ $(OBJ)

rebuild: clean all

SHELLNAME := $(shell uname)
ifeq ($(SHELLNAME), Linux)
	LIBLINK = -L. -l:s21_string.a
else ifeq ($(SHELLNAME), Darwin)
	LIBLINK = s21_string.a
endif

test: $(OBJ) $(OBJ_SPRINTF) s21_string.a test/s21_tests.o
ifeq ($(SHELLNAME), Darwin)
	$(CC) $(BUILD_FLAGS) -o $(EXE_NAME) test/s21_tests.o $(LIBLINK) $(TEST_FLAGS)
else ifeq ($(SHELLNAME), Linux)
	$(CC) $(BUILD_FLAGS) -o $(EXE_NAME) test/s21_tests.o $(LIBLINK) $(TEST_FLAGS) $(LINUX_FLAGS)
endif
	./$(EXE_NAME)

%.o: %.c
	$(CC) -c $(BUILD_FLAGS) $< -o $@

test/s21_tests.o: test/s21_tests.c
	$(CC) -c $(BUILD_FLAGS) $< -o $@

gcov_report: s21_string.a
ifeq ($(SHELLNAME), Darwin)
	$(CC) test/s21_tests.c $(SRC) $(TEST_FLAGS) -coverage -o gcov_report
else ifeq ($(SHELLNAME), Linux)
	$(CC) test/s21_tests.c $(SRC) $(TEST_FLAGS) -coverage -o gcov_report $(LINUX_FLAGS)
endif
	./gcov_report
	lcov -t "s21_test" -o s21_test.info -c -d .
	genhtml -o report s21_test.info
	open ./report/index.html

clean:
	rm -f *.o test/*.o
	rm -f s21_test
	rm -f s21_string.a
	rm -rf report/*
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.gcov
	rm -f test/*.gcda
	rm -f test/*.gcno
	rm -f *.info
	rm -f gcov_report
format:
	clang-format -i -style=Google *.c *.h test/*.c test/*.h

valgrind: test test/s21_tests.o
	CK_FORK=no valgrind $(VALGRIND_FLAGS) --log-file=RESULT_VALGRIND.txt ./$(EXE_NAME)

