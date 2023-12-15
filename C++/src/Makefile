ifeq ($(OS), Windows_NT)
	CC = g++
else
	CC = clang++
endif

COMMON_FLAGS = -std=c++17 -Wall -Werror -Wpedantic
DEBUG_FLAGS = -g -O0
RELEASE_FLAGS = -O3

BIN = ../bin
SRC = *.cpp
EXE = $(BIN)/bfvm
ifeq ($(OS), Windows_NT)
	LIBS = -lkernel32
else
	LIBS = 
endif

$(BIN):
	mkdir $(BIN)

.PHONY: all debug release clean

all: release

debug: $(BIN) |
	$(CC) $(DEBUG_FLAGS) $(COMMON_FLAGS) -o $(EXE) $(SRC) $(LIBS)

release: $(BIN) |
	$(CC) $(RELEASE_FLAGS) $(COMMON_FLAGS) -o $(EXE) $(SRC) $(LIBS)

clean:
	rm -f $(EXE)
