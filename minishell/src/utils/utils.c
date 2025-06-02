#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// 문자열 복사 함수
char *ft_strdup(const char *s) {
    if (!s) return NULL;
    char *copy = malloc(strlen(s) + 1);
    if (!copy) {
        perror("malloc");
        exit(1);
    }
    strcpy(copy, s);
    return copy;
}

// 에러 메시지 출력 후 종료
void fatal_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

// 문자열이 공백인지 확인
int is_whitespace_only(const char *str) {
    while (*str) {
        if (*str != ' ' && *str != '\t' && *str != '\n')
            return 0;
        str++;
    }
    return 1;
}

// 환경변수에서 변수값 얻기
char *get_env_value(const char *name) {
    return getenv(name);  // 또는 custom 구현 가능
}
