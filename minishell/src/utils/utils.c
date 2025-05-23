#include <stdio.h>

#include "../../include/utils.h"
#include "../../include/parser.h"


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
  char *testInput = pos + strlen("ptest ");
  printf("testInput: %s\n", testInput);
  ParsedCommand *testCommand = parseCommand(testInput);

  printf("Keyword: %s\n", testCommand->keyword);
  printf("Arguments Count: %d\n", testCommand->argc);
  printf("Arguments: ");
  ArgNode *current = testCommand->args;
  while (current != NULL) {
    printf("%s ", current->arg);
    current = current->next;
  }
  printf("\n");

  freeParsedCommand(testCommand);
}