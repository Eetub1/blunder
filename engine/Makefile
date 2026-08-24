CC = g++
CFLAGS = -Iinclude -Wall -Wextra -std=c++17
SRC = $(wildcard src/*.cpp)
TARGET = bin/blunder

$(TARGET): $(SRC)
	mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -rf bin