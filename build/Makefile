# Makefile for Tweeter project

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Directories
SRC_DIR = ../src
BUILD_DIR = ../build
INCLUDE_DIR = ../include
FILES_DIR = ../files

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Executable
TARGET = tweeter

# Rules
all: $(BUILD_DIR) $(FILES_DIR) $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(FILES_DIR):
	mkdir -p $(FILES_DIR)

$(BUILD_DIR)/$(TARGET): $(OBJS)
ifeq ($(strip $(OBJS)),)
	@echo "No source files found in $(SRC_DIR)"
else
	$(CC) $(CFLAGS) -o $@ $^
endif

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

clean:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/$(TARGET)

.PHONY: all clean