CC = c++

COMMON_FLAGS = -std=c++23 -Wall -Werror -Wextra -pedantic -fno-exceptions
DEBUG_FLAGS = -g -O0
RELEASE_FLAGS = -O3 -DNDEBUG

BIN = bin
SRC = $(wildcard src/*.cpp)
EXE = $(BIN)/bfvm

.PHONY: all debug release clean

all: debug

release: $(BIN) |
	$(CC) $(RELEASE_FLAGS) $(COMMON_FLAGS) -o $(EXE) $(SRC)

debug: $(BIN) |
	$(CC) $(DEBUG_FLAGS) $(COMMON_FLAGS) -o $(EXE) $(SRC)

clean:
	rm -f $(EXE)

$(BIN):
	mkdir $(BIN)
