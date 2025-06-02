#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

#include "../include/executor.h"
#include "../include/parser.h"
#include "../include/execResult.h"
#include "../include/utils.h"

ExecResult executeExternal(const ParsedCommand *command) {
  // execvp에 넘길 argv 배열 생성
  char **argv = malloc(sizeof(char *) * (command->argc + 2));  // args + keyword + NULL
  if (!argv) {
    perror("memory allocation failed");
    return EXEC_ERROR;
  }

  argv[0] = strdup(command->keyword);

  ArgNode *current = command->args;
  int i = 1;
  while (current) {
    argv[i] = strdup(current->arg);
    i++;
    current = current->next;
  }

  argv[i] = NULL;  // 마지막은 NULL로 종료

  // 자식 프로세스 생성
  pid_t pid = fork();
  if (pid < 0) {
    perror("fork failed");
    return EXEC_ERROR;
  }

  // 자식 프로세스는 이곳으로
  if (pid == 0) {
    // "<" 리디렉션 처리
    if (command->inputFile) {
      int inFd = open(command->inputFile, O_RDONLY);

      if (inFd < 0) {
        perror("open inputFile failed");
        exit(127);
      }

      dup2(inFd, STDIN_FILENO);
      close(inFd);
    }

    // ">" ">>" 리디렉션 처리
    if (command->outputFile) {
      int outFd;
      if (command->isAppend) {
        outFd = open(command->outputFile, O_WRONLY | O_CREAT | O_APPEND, 0644);
      } else {
        outFd = open(command->outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
      }

      if (outFd < 0) {
        perror("open outputFile failed");
        exit(127);
      }

      dup2(outFd, STDOUT_FILENO);
      close(outFd);
    }

    execvp(command->keyword, argv); // execvp로 환경변수를 사용하지 않았는데, 추후 수정 필요
    perror("exec failed");
    exit(127);
  }

  // 부모 프로세스는 자식 종료 대기(& 입력 안 했을 때)
  int status;
  waitpid(pid, &status, 0);

  // 종료 상태 확인
  ExecResult result = EXEC_OK;
  if (WIFEXITED(status)) {
    int exitCode = WEXITSTATUS(status);
    if (exitCode != 0) {
      result = EXEC_ERROR;
    }
  } else {  //자식 프로세스가 exit, return으로 종료되지 않아 status가 없는 경우
    result = EXEC_ERROR;
  }

  // 메모리 할당 해제
  for (int j = 0; j < i; j++) {
    free(argv[j]);
  }
  free(argv);

  return result;
}