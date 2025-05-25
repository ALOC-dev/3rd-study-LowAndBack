CC = gcc
CFLAGS = -Wall -Wextra -g -I./include

SRCS = main.c \
       src/parser/parser.c \
       src/executor/executor.c \
       src/builtin/builtin.c

OBJS = $(SRCS:.c=.o)
TARGET = minishell

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	rm -f $(OBJS) $(TARGET)
