#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../include/builtin.h"
#include "../../include/execResult.h"
#include "../../include/parser.h"
#include "../../include/utils.h"

ExecResult executeBuiltin(ParsedCommand* command) {
  // exit
  if (strcmp(command->keyword, "exit") == 0) {
    if (command->argc == 0) {
      exit(0);
    }
    
    if (command->argc == 1) {
      if (!isNumber(command->args->arg)) {
        fprintf(stderr, "exit: %s: numeric argument required\n", command->args->arg);
        return EXEC_ERROR;
      }

      int exitCode = atoi(command->args->arg);

      if (exitCode < 0 || exitCode > 255) {
        fprintf(stderr, "exit: %d: enter exit code between 0 ~ 255\n", exitCode);
        return EXEC_ERROR;
      }

      exit(exitCode);
    }

    if (command->argc > 1) {
      fprintf(stderr, "exit: too many arguments\n");
      return EXEC_ERROR;
    }
  }

  // cd
  if (strcmp(command->keyword, "cd") == 0) {
    if (command->argc > 1) {
      fprintf(stderr, "cd: too many arguments\n");
      return EXEC_ERROR;
    }

    char *home = getenv("HOME");
    if (command->argc == 0 || strcmp(command->args->arg, "~") == 0) {
      if (chdir(home) != 0) {
        perror("cd");
        return EXEC_ERROR;
      }
    }

    if (command->argc == 1) {
      if (chdir(command->args->arg) != 0) {
        perror("cd");
        return EXEC_ERROR;
      }
    }

    return EXEC_OK;
  }

  // echo
  if (strcmp(command->keyword, "echo") == 0) {
    for (ArgNode* current = command->args; current != NULL; current = current->next) {
      printf("%s", current->arg);
      if (current->next != NULL) {
        printf(" ");
      }
    }
    printf("\n");

    return EXEC_OK;
  }

  // pwd
  if (strcmp(command->keyword, "pwd") == 0) {
    if (command->argc != 0) {
      fprintf(stderr, "pwd: too many arguments\n");
      return EXEC_ERROR;
    }

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
      printf("%s\n", cwd);
    } else {
      perror("pwd");
      return EXEC_ERROR;
    }

    return EXEC_OK;
  }

  // type
  if (strcmp(command->keyword, "type") == 0) {
    



    return EXEC_OK;
  }

  // else
  fprintf(stderr, "%s: command not found\n", command->keyword);
  return EXEC_ERROR;
}