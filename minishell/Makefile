# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

# Directories
SRC_DIR = src
INCLUDE_DIR = include

# Target executable
TARGET = minishell

# Source files grouped by module
BUILTIN_SRC   = $(wildcard $(SRC_DIR)/builtin/*.c)
PARSER_SRC    = $(SRC_DIR)/parser/parser.c
EXECUTOR_SRC  = $(wildcard $(SRC_DIR)/executor/*.c)
UTILS_SRC     = $(wildcard $(SRC_DIR)/utils/*.c)
MAIN_SRC      = $(SRC_DIR)/main.c

# All source files
SRC = $(BUILTIN_SRC) $(PARSER_SRC) $(EXECUTOR_SRC) $(UTILS_SRC) $(MAIN_SRC)

# Object files (replace .c with .o)
OBJ = $(SRC:.c=.o)

# Default target
.PHONY: all
all: $(TARGET)

# Link objects into the final executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

# Generic rule to build object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files
.PHONY: clean
clean:
	rm -f $(OBJ)

# Clean up everything, including the executable
.PHONY: fclean
fclean: clean
	rm -f $(TARGET)

# Rebuild from scratch
.PHONY: re
re: fclean all
