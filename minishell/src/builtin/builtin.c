#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

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

    if (command->argc == 1) {
      char *targetPath = NULL;

      if (strcmp(command->args->arg, "~") == 0) {
        targetPath = getenv("HOME");
      } else {
        targetPath = command->args->arg;
      }

      if (chdir(targetPath) != 0) {
        perror("cd");
        return EXEC_ERROR;
      }
    }

    return EXEC_OK;
  }

  // echo
  if (strcmp(command->keyword, "echo") == 0) {
    // ">" ">>" 리디렉션 처리
    if (command->outputFile != NULL) {
      int outFd = open(command->outputFile,
                   command->isAppend ? O_WRONLY | O_CREAT | O_APPEND : O_WRONLY | O_CREAT | O_TRUNC,
                   0644);
      if (outFd < 0) {
        perror("echo");
        return EXEC_ERROR;
      }

      for (ArgNode* current = command->args; current != NULL; current = current->next) {
        write(outFd, current->arg, strlen(current->arg));
        if (current->next != NULL) {
          write(outFd, " ", 1);
        }
      }
      write(outFd, "\n", 1);
      close(outFd);
      
      return EXEC_OK;
    }

    // 리디렉션 없는 경우
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
      // ">" ">>" 리디렉션 처리
      if (command->outputFile != NULL) {
        int outFd = open(command->outputFile,
                     command->isAppend ? O_WRONLY | O_CREAT | O_APPEND : O_WRONLY | O_CREAT | O_TRUNC,
                     0644);
        if (outFd < 0) {
          perror("pwd");
          return EXEC_ERROR;
        }
        write(outFd, cwd, strlen(cwd));
        write(outFd, "\n", 1);
        close(outFd);
      }

      // 리디렉션 없는 경우
      printf("%s\n", cwd);
    } else {
      perror("pwd");
      return EXEC_ERROR;
    }

    return EXEC_OK;
  }

  // type
  if (strcmp(command->keyword, "type") == 0) {
    if (command->argc == 0) {
      fprintf(stderr, "type: missing operand\n");
      return EXEC_ERROR;
    }

    for (ArgNode* current = command->args; current != NULL; current = current->next) {
      if (isBuiltin(current->arg)) {
        printf("%s is a shell builtin\n", current->arg);
        continue;
      }
      
      char *pathEnv = getenv("PATH");
      if (!pathEnv) {
        printf("%s: command not found\n", current->arg);
        continue;
      }

      char *pathCopy = strdup(pathEnv);
      char *dir = strtok(pathCopy, ":");
      int found = 0;

      while (dir != NULL) {
        char fullPath[1024];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", dir, current->arg);

        if (access(fullPath, X_OK) == 0) {
          printf("%s is %s\n", current->arg, fullPath);
          found = 1;
          break;
        }

        dir = strtok(NULL, ":");
      }

      if (!found) {
        printf("%s: command not found\n", current->arg);
      }

      free(pathCopy);
    }

    return EXEC_OK;
  }

  // else
  fprintf(stderr, "%s: command not found\n", command->keyword);
  return EXEC_ERROR;
}