CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c11
TARGET=s21_cat
SRC=s21_cat.c

all: clean $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

test: all
	sh test_cat.sh

style:
	clang-format -style=Google -n *.c *.h

clean:
	rm -f $(TARGET)