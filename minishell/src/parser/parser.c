#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/parser.h"

void initParsedCommand(ParsedCommand* command) {
  command->keyword = NULL;
  command->args = NULL;
  command->argc = 0;
  command->next = NULL;
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
    return; // 에러는 makenode()에서 처리
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

ParsedCommand* parseCommand(char *input) {
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

  while ((token = strtok(NULL, " ")) != NULL) {
    appendArg(command, token);
  }

  return command;
}

void freeParsedCommand(ParsedCommand* command) {
  if (command == NULL) return;

  ArgNode* current = command->args;
  while (current != NULL) {
    ArgNode* temp = current;
    current = current->next;
    free(temp->arg);
    free(temp);
  }

  free(command->keyword);
  free(command);
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