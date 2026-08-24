CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -std=c11
SRC = $(wildcard src/*.c)
TARGET = bin/blunder

$(TARGET): $(SRC)
	mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -rf bin