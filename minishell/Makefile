# ────────── 컴파일 설정 ──────────
CC     = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude

# ────────── 소스 코드 ──────────
SRCS = \
	main.c \
	src/builtin/builtin_cd.c \
	src/builtin/builtin_echo.c \
	src/builtin/builtin_exit.c \
	src/builtin/builtin_pwd.c \
	src/builtin/builtin_type.c \
	src/executor/execute.c \
	src/parser/parser.c

OBJS = $(SRCS:.c=.o)

# ────────── 실행 파일 이름 ────────
NAME = minishell

# ────────── 규칙 ──────────
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re