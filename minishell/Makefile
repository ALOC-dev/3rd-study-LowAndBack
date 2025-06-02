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

# 오브젝트 파일 목록 (build 디렉토리에 저장)
OBJS = $(SRCS:.c=.o)

# 기본 규칙
all: $(NAME)

# 실행 파일 생성
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# .o 파일 생성 규칙 (중첩 디렉토리 대응)
%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# 정리
clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
