CC = cc

CFLAGS = -Wall -Werror -Wpedantic -Wextra -std=c99
ifeq ($(MAKECMDGOALS), release)
	CFLAGS += -O3 -DNDEBUG
endif

SRC_DIR = src
BIN_DIR = bin
LOCALBIN = ~/.local/bin

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst %.c, $(BIN_DIR)/%.o, $(SRC_FILES))

TARGET = $(BIN_DIR)/bfvm

.PHONY: all release install uninstall clean

all: $(TARGET)

release: all

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

$(BIN_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

install:
	mkdir -p $(LOCALBIN)
	install $(TARGET) $(LOCALBIN)

uninstall:
	rm -f $(LOCALBIN)/bfvm

clean:
	rm -f $(TARGET) $(OBJ_FILES)

