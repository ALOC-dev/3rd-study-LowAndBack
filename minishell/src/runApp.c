#include <stdio.h>
#include <stdlib.h>

#include "../include/parser.h"
#include "../include/execResult.h"

#define true 1

// 입력을 버퍼에 저장
char *readLine() {
  size_t bufferSize = 256;  // 초기 버퍼 사이즈
  size_t length = 0;

  char *buffer = malloc(bufferSize);
  // 할당 실패 시
  if (!buffer) {
    fprintf(stderr, "Failed to allocate input buffer\n");
    exit(1);
  }

  int character;
  while ((character = getchar()) != '\n' && character != EOF) {
    buffer[length] = character;
    length++;

    // 버퍼의 마지막에 도달한 경우 길이를 늘려 재할당
    if (length >= bufferSize) {
      bufferSize *= 2;
      buffer = realloc(buffer, bufferSize);
      // 재할당 실패 시
      if (!buffer) {
        fprintf(stderr, "Failed to reallocate input buffer\n");
        exit(1);
      }
    }
  }

  // newLine 또는 EOF에 도달한 경우
  buffer[length] = '\0';
  return buffer;
}

int runApp(int argc, char **argv, char **envp) {
  while (true) {
    // 프롬프트 출력
    printf("minishell > ");

    // 입력을 읽은 뒤 parsing
    char *input = readLine();
    ParsedCommand *command = parseCommand(input);
    
    // 내부/외장 명령 실행
    ExecResult result;
    if (isBuiltin(command->keyword)) {
      result = executeBuiltin(command);
    } else {
      result = executeExternal(command);
    }

    // 메모리 할당 해제
    freeParsedCommand(command);
    free(input);

    // exit
    if (result == EXEC_EXIT) break;
  }

  return 0;
}