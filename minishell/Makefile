CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

NAME = minishell

SRCS = main.c executor.c builtin.c parser.c
OBJS = $(SRCS:.c=.o)

HEADERS = executor.h builtin.h parser.h

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
