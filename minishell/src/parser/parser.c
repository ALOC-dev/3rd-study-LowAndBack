#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/parser.h"

void initParsedCommand(ParsedCommand* command) {
  command->keyword = NULL;
  command->args = NULL;
  command->argc = 0;
  command->next = NULL;
  command->inputFile = NULL;
  command->outputFile = NULL;
  command->isAppend = 0;
}

ArgNode* makeNode(char* arg) {
  ArgNode* newNode = malloc(sizeof(ArgNode));
  if (!newNode) {
    fprintf(stderr, "Failed to allocate ArgNode\n");
    return NULL;
  }
  newNode->arg = strdup(arg);
  newNode->next = NULL;
  return newNode;
}

void appendArg(ParsedCommand* command, char* arg) {
  ArgNode* newNode = makeNode(arg);
  if (!newNode) {
    return;
  }

  if (command->args == NULL) {
    command->args = newNode;
  } else {
    ArgNode* current = command->args;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = newNode;
  }
  command->argc++;
}

// 파이프 포함한 명령어 한 줄 모두 parsing
ParsedCommand* parseCommand(char *input) {
  // 이중 연결 리스트로 파싱된 명령어를 저장
  ParsedCommand* head = NULL;
  ParsedCommand* tail = NULL;

  char* saveptr;
  char* commandStr = strtok_r(input, "|", &saveptr);

  while (commandStr != NULL) {
    ParsedCommand* command = parseSingleCommand(commandStr);
    if (command == NULL) {
      fprintf(stderr, "parse error\n");
      freeParsedCommand(head);
      return NULL;
    }

    if (head == NULL) {
      head = command;
      tail = command;
    } else {
      tail->next = command;
      tail = command;
    }

    commandStr = strtok_r(NULL, "|", &saveptr);
  }

  return head;
}

// 단일 명령어 parsing
ParsedCommand* parseSingleCommand(char *input) {
  ParsedCommand* command = malloc(sizeof(ParsedCommand));
  if (!command) {
    fprintf(stderr, "Failed to allocate ParsedCommand\n");
    return NULL;
  }

  initParsedCommand(command);

  char* token = strtok(input, " ");
  if (token) {
    command->keyword = strdup(token);

    if (!command->keyword) {
      fprintf(stderr, "Failed to strdup keyword\n");
      free(command);
      return NULL;
    }
  } else {
    free(command);
    return NULL;
  }

  // 두 번째 인자부터 처리
  // 리디렉션은 입력과 출력을 각각 최대 1개씩만 허용
  while ((token = strtok(NULL, " ")) != NULL) {
    if (strcmp(token, "<") == 0) {
      command->isAppend = 0;
      token = strtok(NULL, " ");
      if (token) {
        free(command->inputFile); // 기존 inputFile 있다면 해제
        command->inputFile = strdup(token);
        if (!command->inputFile) {
          fprintf(stderr, "Failed to strdup inputFile\n");
          freeParsedCommand(command);
          return NULL;
        }
      }
    } else if (strcmp(token, ">") == 0) {
      command->isAppend = 0;
      token = strtok(NULL, " ");
      if (token) {
        free(command->outputFile);
        command->outputFile = strdup(token);
        if (!command->outputFile) {
          fprintf(stderr, "Failed to strdup outputFile\n");
          freeParsedCommand(command);
          return NULL;
        }
      }
    } else if (strcmp(token, ">>") == 0) {
      command->isAppend = 1;
      token = strtok(NULL, " ");
      if (token) {
        free(command->outputFile);
        command->outputFile = strdup(token);
        if (!command->outputFile) {
          fprintf(stderr, "Failed to strdup outputFile for append\n");
          freeParsedCommand(command);
          return NULL;
        }
      }
    } else {
      appendArg(command, token);
    }
  }

  return command;
}

void freeParsedCommand(ParsedCommand* command) {
  while (command != NULL) {
    ParsedCommand* next = command->next;

    ArgNode* current = command->args;
    while (current != NULL) {
      ArgNode* temp = current;
      current = current->next;
      free(temp->arg);
      free(temp);
    }

    free(command->keyword);
    free(command->inputFile);
    free(command->outputFile);
    free(command);

    command = next;
  }
}

void printParsedCommand(const ParsedCommand* command) {
  if (command == NULL) {
    printf("ParsedCommand is NULL\n");
    return;
  }

  printf("Keyword: %s\n", command->keyword);
  printf("Arguments: ");
  ArgNode* current = command->args;
  while (current != NULL) {
    printf("%s ", current->arg);
    current = current->next;
  }
  printf("\n");
}