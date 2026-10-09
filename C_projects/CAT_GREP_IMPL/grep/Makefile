CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c11
TARGET=s21_grep
SRC=s21_grep.c

all: clean $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

test: all
	sh test_grep.sh

style:
	clang-format -style=Google -n *.c *.h

clean:
	rm -f $(TARGET)