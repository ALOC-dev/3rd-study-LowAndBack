#ifndef BUILTIN_H
#define BUILTIN_H
#include <stdbool.h>

/**
 * 내장 명령어인지 검사하는 함수
 * @param cmd 명령어 문자열
 * @return 내장 명령어면 true, 아니면 false
 */
bool is_builtin_command(const char *cmd);

/**
 * 내장 명령어 실행 함수
 * @param args 명령어 및 인자 배열 (NULL 종료)
 * @return 성공하면 0, 실패하면 음수 반환
 */
int execute_builtin(char *const args[]);

/* 내장 명령어별 함수 프로토타입 */
int builtin_cd(char *const args[]);
int builtin_exit(char *const args[]);
int builtin_pwd(char *const args[]);
int builtin_echo(char *const args[]);
int builtin_type(char *const args[]);

#endif // BUILTIN_H
