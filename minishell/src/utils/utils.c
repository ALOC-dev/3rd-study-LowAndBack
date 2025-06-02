#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>

#include "../../include/utils.h"
#include "../../include/parser.h"
#include "../../include/builtinList.h"

int isNumber(const char* str) {
  if (*str == '\0') return 0; // 빈 문자열

  if (*str == '-' || *str == '+') str++;  // 부호 처리

  if (*str == '\0') return 0; // 부호만 있었던 경우

  while (*str) {
    if (*str < '0' || *str > '9') return 0; // 숫자가 아닌 경우
    str++;
  }
  return 1; // 모두 숫자인 경우
}

void testParsingCommand(char* input) {
  char *pos = strstr(input, "ptest ");
  if (!pos) {
    fprintf(stderr, "Invalid ptest command format\n");
    return;
  }

  char *testInput = pos + strlen("ptest ");
  printf("testInput: %s\n", testInput);

  ParsedCommand *cmd = parseCommand(testInput);
  if (!cmd) {
    fprintf(stderr, "Parsing failed.\n");
    return;
  }

  int cmdNum = 1;
  while (cmd) {
    printf("\n--- Command %d ---\n", cmdNum++);
    printf("Keyword: %s\n", cmd->keyword);
    printf("Arguments Count: %d\n", cmd->argc);
    printf("Arguments: ");
    ArgNode *current = cmd->args;
    while (current) {
      printf("%s ", current->arg);
      current = current->next;
    }
    printf("\n");

    if (cmd->inputFile) {
      printf("Input Redirection: %s\n", cmd->inputFile);
    }

    if (cmd->outputFile) {
      printf("Output Redirection: %s (%s)\n",
             cmd->outputFile,
             cmd->isAppend ? "append" : "overwrite");
    }

    cmd = cmd->next;
  }

  freeParsedCommand(parseCommand(testInput));
}

int isBuiltin(const char *keyword) {
  int builtinCount = sizeof(builtinList) / sizeof(builtinList[0]);
  for (int i = 0; i < builtinCount; i++) {
    if (strcmp(keyword, builtinList[i]) == 0) {
      return 1;  // 내장 명령어
    }
  }
  return 0;
}

int isExternalCommand(const char *command) {
  if (strchr(command, '/')) {
    return access(command, X_OK) == 0;
  }

  char *pathEnv = getenv("PATH");
  if (!pathEnv) return 0;

  char *paths = strdup(pathEnv);
  char *dir = strtok(paths, ":");

  while (dir) {
    char fullPath[1024];
    snprintf(fullPath, sizeof(fullPath), "%s/%s", dir, command);
    if (access(fullPath, X_OK) == 0) {
      free(paths);
      return 1;
    }
    dir = strtok(NULL, ":");
  }

  free(paths);
  return 0;
}

int isOnlyWhitespace(const char *str) {
  while (*str) {
    if (!isspace((unsigned char)*str)) return 0;
    str++;
  }
  return 1;
}