#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#include "../include/executor.h"
#include "../include/parser.h"
#include "../include/execResult.h"

ExecResult executeExternal(ParsedCommand *cmd) {
  // 1. execvp에 넘길 argv 배열 생성
  char **argv = malloc(sizeof(char *) * (cmd->argc + 2));  // +1 for keyword, +1 for NULL
  if (!argv) {
    perror("malloc failed");
    return EXEC_ERROR;
  }

  argv[0] = strdup(cmd->keyword);  // 명령어 자체
  ArgNode *current = cmd->args;
  int i = 1;
  while (current) {
    argv[i++] = strdup(current->arg);
    current = current->next;
  }
  argv[i] = NULL;  // 마지막은 NULL로 종료

  // 2. 자식 프로세스 생성
  pid_t pid = fork();
  if (pid < 0) {
    perror("fork failed");
    return EXEC_ERROR;
  }

  if (pid == 0) {
    // 자식 프로세스에서 명령 실행
    execvp(cmd->keyword, argv);
    perror("execvp failed");  // exec 실패 시만 도달
    exit(127);  // "command not found"에 해당하는 종료 코드
  }

  // 3. 부모 프로세스는 자식 종료 대기
  int status;
  waitpid(pid, &status, 0);

  // 4. 종료 코드 확인
  ExecResult result = EXEC_OK;
  if (WIFEXITED(status)) {
    int exitCode = WEXITSTATUS(status);
    if (exitCode != 0) {
      result = EXEC_ERROR;
    }
  } else {
    result = EXEC_ERROR;
  }

  // 5. 메모리 정리
  for (int j = 0; j < i; j++) {
    free(argv[j]);
  }
  free(argv);

  return result;
}