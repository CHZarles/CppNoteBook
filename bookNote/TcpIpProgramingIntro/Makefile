# Compiler and flags
CC = gcc
CXX = g++
CFLAGS = -Wall -g
CPPFLAGS = -Wall -g
INCLUDES = -Icommon

# Directories
SRC_DIRS = ch01 ch02 common ch03 ch04 ch05 ch06 ch07 ch08 ch09
BUILD_DIR = build
BIN_DIR = bin

# Find all source files
C_SRCS = $(shell find $(SRC_DIRS) -name '*.c')
CPP_SRCS = $(shell find $(SRC_DIRS) -name '*.cpp')

# Object files and executables
C_OBJS = $(C_SRCS:%.c=$(BUILD_DIR)/%.o)
CPP_OBJS = $(CPP_SRCS:%.cpp=$(BUILD_DIR)/%.o)
EXECUTABLES = $(patsubst %.c,$(BIN_DIR)/%,$(C_SRCS)) $(patsubst %.cpp,$(BIN_DIR)/%,$(CPP_SRCS))

# Default target
all: $(EXECUTABLES)

# Rule to build object files from C sources
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Rule to build object files from C++ sources
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(INCLUDES) -c $< -o $@

# Rule to build executables
$(BIN_DIR)/%: $(BUILD_DIR)/%.o
	@mkdir -p $(dir $@)  # Ensure target directory exists
	$(CC) $(CFLAGS) $< -o $@

# Clean up
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean


