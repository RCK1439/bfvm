CC = clang++

COMMON_FLAGS = -std=c++17 -Wall -Werror -Wpedantic
DEBUG_FLAGS = -g -O0
RELEASE_FLAGS = -O3

BIN = bin
SRC = $(wildcard src/*.cpp)
EXE = $(BIN)/bfvm

$(BIN):
	mkdir $(BIN)

.PHONY: all debug release clean

all: release

release: $(BIN) |
	$(CC) $(RELEASE_FLAGS) $(COMMON_FLAGS) -o $(EXE) $(SRC)

debug: $(BIN) |
	$(CC) $(DEBUG_FLAGS) $(COMMON_FLAGS) -o $(EXE) $(SRC)

clean:
	rm -f $(EXE)
