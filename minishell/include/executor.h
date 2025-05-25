// executor.h
#ifndef EXECUTOR_H
#define EXECUTOR_H

/**
 * 외부 명령어를 실행하는 함수
 * @param args 명령어 및 인자를 포함하는 NULL 종료 문자열 배열
 */
void execute_command(char *const args[]);

#endif // EXECUTOR_H
