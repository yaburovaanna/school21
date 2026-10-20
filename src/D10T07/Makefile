CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

BUILD_DIR = build
TARGET = $(BUILD_DIR)/Quest_1
SRC = src/dmanager_module.c
HEADER = src/door_struct.h

.PHONY: door_struct clean

door_struct: $(TARGET)

$(TARGET): $(SRC) $(HEADER)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -rf $(BUILD_DIR)