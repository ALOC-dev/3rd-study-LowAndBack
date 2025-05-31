CC = gcc
CFLAGS = -Wall -Wextra -g -I./include

SRCS = main.c \
       src/parser/parser.c \
       src/executor/executor.c \
       src/builtin/builtin.c

OBJS = $(SRCS:%.c=build/%.o)
TARGET = minishell

# 빌드 폴더와 서브폴더를 자동으로 생성
$(shell mkdir -p build/src/parser build/src/executor build/src/builtin)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

build/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build $(TARGET)

