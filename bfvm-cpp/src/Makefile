CC = clang++

CFLAGS = -Wall -Werror -Wextra -Wpedantic -std=c++23 -fno-exceptions
ifeq ($(MAKECMDGOALS), release)
	CFLAGS += -O3 -DNDEBUG
else
	CFLAGS += -O0 -g
endif

SRC_DIR = .
COMPILER_DIR = $(SRC_DIR)/Compiler
BIN_DIR = ../bin

SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp $(COMPILER_DIR)/*.cpp)
OBJ_FILES = $(patsubst %.cpp, $(BIN_DIR)/%.o, $(SRC_FILES))

INCL = -I$(SRC_DIR)

TARGET = $(BIN_DIR)/bfvm
ifeq ($(OS), Windows_NT)
	TARGET = $(BIN_DIR)/bfvm.exe
endif

.PHONY: all release clean

all: $(TARGET)

release: all

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

$(BIN_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $^ -o $@ $(INCL)

clean:
	rm -f $(TARGET) $(OBJ_FILES)
