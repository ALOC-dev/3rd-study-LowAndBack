# **************************************************************************** #
#                              MINISHELL MAKEFILE                              #
# **************************************************************************** #

NAME = minishell

# 컴파일러 및 플래그
CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude

# 소스 파일 목록
SRCS = \
	src/main.c \
	src/builtin/builtin.c \
	src/parser/parser.c \
	src/executor/executor.c

# 오브젝트 파일
OBJS = $(SRCS:.c=.o)

# 기본 규칙
all: $(NAME)

# 실행 파일 링크
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# 객체 파일 규칙
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 클린
clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
