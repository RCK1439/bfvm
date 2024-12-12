CC = c++

CFLAGS = -Wall -Werror -Wextra -Wpedantic -fno-exceptions -std=c++23
ifeq ($(MAKECMDGOALS), release)
	CFLAGS += -O3 -DNDEBUG
endif

SRC_DIR = src
BIN_DIR = bin

SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst %.cpp, $(BIN_DIR)/%.o, $(SRC_FILES))

TARGET = $(BIN_DIR)/bfvm

.PHONY: all release clean

all: $(TARGET)

release: all

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

$(BIN_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f $(TARGET) $(OBJ_FILES)
