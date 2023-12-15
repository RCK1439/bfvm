CC = clang

COMMON_FLAGS = -Wall -Werror -Wpedantic
DEBUG_FLAGS = -ggdb -O0
RELEASE_FLAGS = -O3

BIN = ../bin
LOCALBIN = ~/.local/bin
SRC = $(wildcard *.c)
EXE = $(BIN)/bfvm

$(BIN):
	mkdir $(BIN)

.PHONY: all debug release install uninstall clean

all: release

debug: $(BIN) |
	$(CC) $(DEBUG_FLAGS) $(COMMON_FLAGS) -o $(EXE) $(SRC)

release: $(BIN) |
	$(CC) $(RELEASE_FLAGS) $(COMMON_FLAGS) -o $(EXE) $(SRC)

install:
	mkdir -p $(LOCALBIN)
	install $(EXE) $(LOCALBIN)

uninstall:
	rm -f $(LOCALBIN)/bfvm

clean:
	rm -f $(EXE)