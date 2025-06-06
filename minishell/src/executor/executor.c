#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include "executor.h"
#include "builtin.h"

void execute_single_command(ParsedCommand *cmd) {
    char **args = malloc(sizeof(char *) * (cmd->argc + 1));
    if (!args) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    ArgNode *cur = cmd->args;
    int i = 0;
    while (cur) {
        args[i++] = cur->arg;
        cur = cur->next;
    }
    args[i] = NULL;

    // 리다이렉션 처리
    if (cmd->infile) {
        int fd = open(cmd->infile, O_RDONLY);
        if (fd < 0) {
            perror("open infile");
            free(args);
            exit(EXIT_FAILURE);
        }
        if (dup2(fd, STDIN_FILENO) < 0) {
            perror("dup2 infile");
            close(fd);
            free(args);
            exit(EXIT_FAILURE);
        }
        close(fd);
    }

    if (cmd->outfile) {
        int fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) {
            perror("open outfile");
            free(args);
            exit(EXIT_FAILURE);
        }
        if (dup2(fd, STDOUT_FILENO) < 0) {
            perror("dup2 outfile");
            close(fd);
            free(args);
            exit(EXIT_FAILURE);
        }
        close(fd);
    } else if (cmd->appendfile) {
        int fd = open(cmd->appendfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd < 0) {
            perror("open appendfile");
            free(args);
            exit(EXIT_FAILURE);
        }
        if (dup2(fd, STDOUT_FILENO) < 0) {
            perror("dup2 appendfile");
            close(fd);
            free(args);
            exit(EXIT_FAILURE);
        }
        close(fd);
    }

    // 내장 명령어 실행
    if (is_builtin_command(args[0])) {
        int ret = execute_builtin(args);
        free(args);
        exit(ret);
    }

    execvp(args[0], args);
    perror("execvp failed");
    free(args);
    exit(EXIT_FAILURE);
}

int execute_command(ParsedCommand *cmd) {
    int fd[2];
    int in_fd = STDIN_FILENO;
    pid_t pid;
    int status;

    ParsedCommand *current = cmd;
    while (current) {
        if (current->next) {
            if (pipe(fd) < 0) {
                perror("pipe failed");
                exit(EXIT_FAILURE);
            }
        }

        pid = fork();
        if (pid < 0) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // 자식 프로세스

            if (dup2(in_fd, STDIN_FILENO) < 0) {
                perror("dup2 in_fd");
                exit(EXIT_FAILURE);
            }

            if (current->next && !current->outfile && !current->appendfile) {
                if (dup2(fd[1], STDOUT_FILENO) < 0) {
                    perror("dup2 pipe write");
                    exit(EXIT_FAILURE);
                }
            }

            if (in_fd != STDIN_FILENO)
                close(in_fd);
            if (current->next) {
                close(fd[0]);
                close(fd[1]);
            }

            execute_single_command(current);
        } else {
            // 부모 프로세스

            if (in_fd != STDIN_FILENO)
                close(in_fd);
            if (current->next) {
                close(fd[1]);
                in_fd = fd[0];
            }
        }

        current = current->next;
    }

    // 백그라운드 여부 확인
    if (!cmd->is_background) {
        // 포그라운드 실행이면 wait
        while (wait(&status) > 0);
    } else {
        printf("[BG] pid: %d\n", pid);
    }

    return 0;
}
