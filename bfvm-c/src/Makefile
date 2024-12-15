CC = clang

CFLAGS = -Wall -Werror -Wpedantic -Wextra -std=c99
ifeq ($(MAKECMDGOALS), release)
	CFLAGS += -O3 -DNDEBUG
else
	CFLAGS += -O0 -g
endif

SRC_DIR = .
BFC_DIR =  $(SRC_DIR)/bfc
BIN_DIR = ../bin

SRC_FILES = $(wildcard $(SRC_DIR)/*.c $(BFC_DIR)/*.c)
OBJ_FILES = $(patsubst %.c, $(BIN_DIR)/%.o, $(SRC_FILES))

INCL = -I$(SRC_DIR)

ifeq ($(OS), Windows_NT)
	TARGET = $(BIN_DIR)/bfvm.exe
else
	TARGET = $(BIN_DIR)/bfvm
endif

.PHONY: all release clean

all: $(TARGET)

release: all

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

$(BIN_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCL)

clean:
	rm -f $(TARGET) $(OBJ_FILES)

