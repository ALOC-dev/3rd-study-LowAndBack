CC = gcc

CFLAGS = -Wall -Wextra -Iinclude

SRC = src/minishell.c src/runApp.c src/builtin/builtin.c src/executor/executor.c src/parser/parser.c src/utils/utils.c

OBJ = $(SRC:.c=.o)

TARGET = minishell

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)