GCC      := g++
GCCFLAGS := -std=c++17 -Iinclude

SRCS := $(wildcard src/*.cpp)
OBJS := $(SRCS:src/%.cpp=src/%.o)
TARGET := bin/cpp_engine

all: $(TARGET)

$(TARGET): $(OBJS) | bin
	$(GCC) $(OBJS) -o $(TARGET)

src/%.o: src/%.cpp
	$(GCC) $(GCCFLAGS) -c $< -o $@

bin:
	mkdir -p bin

clean:
	rm -rf src/*.o bin