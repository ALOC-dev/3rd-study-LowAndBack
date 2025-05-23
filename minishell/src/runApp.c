#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/parser.h"
#include "../include/execResult.h"
#include "../include/builtinList.h"
#include "../include/builtin.h"
#include "../include/executor.h"

#define true 1
#define false 0

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

  // newLine에 도달한 경우
  if (length > 0 && buffer[length - 1] == '\n') {
    buffer[length - 1] = '\0';  // newLine 제거
  }
  return buffer;
}

int isBuiltin(const char *keyword) {
  int builtinCount = sizeof(builtinList) / sizeof(builtinList[0]);
  for (int i = 0; i < builtinCount; i++) {
    if (strcmp(keyword, builtinList[i]) == 0) {
      return true;  // 내장 명령어
    }
  }
  return false;  // 외장 명령어
}

int runApp(int argc, char **argv, char **envp) {
  while (true) {
    // 프롬프트 출력
    printf("minishell > ");
    fflush(stdout);

    // 입력을 읽은 뒤 parsing
    char *input = readLine();
    ParsedCommand *command = parseCommand(input);

    // 디버깅용
    if (strcmp(command->keyword, "ptest") == 0) {
      testParsingCommand(input);
      
      freeParsedCommand(command);
      free(input);
      continue;
    }
    
    // 내부/외장 명령 실행
    ExecResult result;
    if (isBuiltin(command->keyword)) {
      result = executeBuiltin(command);
    } else {
      result = executeExternal(command);
    }

    // 명령어 실행 실패
    if (result == EXEC_ERROR) {
      fprintf(stderr, "Error occured while executing command: %s\n", command->keyword);
    }

    // 메모리 할당 해제
    freeParsedCommand(command);
    free(input);
  }

  return 0;
}