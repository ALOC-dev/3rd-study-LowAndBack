#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include "executor.h"

void execute_single_command(ParsedCommand *cmd) {
    char **args = malloc(sizeof(char *) * (cmd->argc + 2));
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

    // 입력 리다이렉션
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

    // 출력 리다이렉션 (덮어쓰기)
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
    } 
    // 출력 리다이렉션 (추가쓰기)
    else if (cmd->appendfile) {
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

    execvp(args[0], args);
    perror("execvp failed");
    free(args);
    exit(EXIT_FAILURE);
}

void execute_commands(ParsedCommand *cmd) {
    int fd[2];
    int in_fd = STDIN_FILENO;  // 처음 stdin은 표준 입력

    while (cmd) {
        if (cmd->next) {
            if (pipe(fd) < 0) {
                perror("pipe failed");
                exit(EXIT_FAILURE);
            }
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // 자식 프로세스

            // 이전 명령어의 출력(또는 stdin)을 현재 명령어의 stdin으로 연결
            if (dup2(in_fd, STDIN_FILENO) < 0) {
                perror("dup2 in_fd");
                exit(EXIT_FAILURE);
            }

            // 파이프가 있으면 출력은 파이프 쓰기 끝으로 연결
            if (cmd->next) {
                if (dup2(fd[1], STDOUT_FILENO) < 0) {
                    perror("dup2 pipe write");
                    exit(EXIT_FAILURE);
                }
            }

            // 파이프 읽기 끝 닫기
            if (cmd->next) {
                close(fd[0]);
                close(fd[1]);
            }

            execute_single_command(cmd);
        } else {
            // 부모 프로세스

            // 이전에 쓴 fd 닫기
            if (in_fd != STDIN_FILENO) close(in_fd);
            if (cmd->next) {
                close(fd[1]);  // 쓰기 끝 닫기
                in_fd = fd[0]; // 읽기 끝 다음 명령어 stdin으로
            }

            // 자식 프로세스 기다리기
            int status;
            if (waitpid(pid, &status, 0) < 0) {
                perror("waitpid failed");
                exit(EXIT_FAILURE);
            }
        }

        cmd = cmd->next;
    }
}
