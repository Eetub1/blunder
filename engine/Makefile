CC = g++
CFLAGS = -Iinclude -Wall -Wextra -std=c++17
SRC = $(wildcard src/*.cpp)
HEADERS = $(wildcard include/*.hpp)
TARGET = bin/blunder

$(TARGET): $(SRC) $(HEADERS)
	mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -rf bin