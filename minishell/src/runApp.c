#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../include/parser.h"
#include "../include/execResult.h"
#include "../include/builtin.h"
#include "../include/executor.h"
#include "../include/utils.h"

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

int runApp(int argc, char **argv, char **envp) {
  while (1) {
    // 프롬프트 출력
    printf("minishell > ");
    fflush(stdout);

    // 입력을 읽은 뒤 parsing
    char *input = readLine();

    char *inputCopy = strdup(input);  // testParsingCommand()에서 사용
    ParsedCommand *command = parseCommand(input);
    if (!command) {
      free(inputCopy);
      free(input);
      continue;
    }

    // 디버깅용
    if (strcmp(command->keyword, "ptest") == 0) {
      testParsingCommand(inputCopy);
      freeParsedCommand(command);
      free(inputCopy);
      free(input);
      continue;
    }
    
    // 내부/외장 명령 실행
    ExecResult result;
    if (isBuiltin(command->keyword)) {
      result = executeBuiltin(command);
    } else if (isExternalCommand(command->keyword)) {
      result = executeExternal(command);
    } else {
      fprintf(stderr, "%s: command not found\n", command->keyword);
      result = EXEC_ERROR;
    }

    // 명령어 실행 실패
    if (result == EXEC_ERROR) {
      //fprintf(stderr, "Error occured while executing command: %s\n", command->keyword); // 중복으로 뜸 -> 나중에 에러 처리 잘 할 것
    }

    // 메모리 할당 해제
    freeParsedCommand(command);
    free(inputCopy);
    free(input);
  }

  return 0;
}