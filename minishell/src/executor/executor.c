#include "executor.h"
#include "builtin.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

void handle_signal(int sig) {
    if (sig == SIGINT)
        write(1, "\n$ ", 3);
}

void execute_command(t_command *cmd, int input_fd, int is_last) {
    int pipe_fd[2];
    pid_t pid;
    int status;

    // 다음 명령이 있으면 파이프 생성
    if (!is_last && pipe(pipe_fd) < 0) {
        perror("pipe");
        return;
    }

    pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        // 자식 프로세스

        // 시그널 기본 설정
        signal(SIGINT, SIG_DFL);

        // 이전 명령에서 전달받은 입력이 있다면 연결
        if (input_fd != STDIN_FILENO) {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }

        // 파이프 출력 설정
        if (!is_last) {
            dup2(pipe_fd[1], STDOUT_FILENO);
            close(pipe_fd[0]);
            close(pipe_fd[1]);
        }

        // 1. 입력 리다이렉션
        if (cmd->infile) {
            int fd_in = open(cmd->infile, O_RDONLY);
            if (fd_in < 0) {
                perror("open infile");
                exit(1);
            }
            dup2(fd_in, STDIN_FILENO);
            close(fd_in);
        }

        // 2. 출력 리다이렉션
        if (cmd->outfile) {
            int fd_out = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd_out < 0) {
                perror("open outfile");
                exit(1);
            }
            dup2(fd_out, STDOUT_FILENO);
            close(fd_out);
        }

        // 3. builtin
        if (is_builtin(cmd->keyword)) {
            run_builtin(cmd);
            exit(0);
        }

        // 4. 외부 명령어 실행
        execvp(cmd->keyword, cmd->args);
        perror("execvp");
        exit(1);
    } else {
        // 부모 프로세스

        if (input_fd != STDIN_FILENO)
            close(input_fd);
        if (!is_last) {
            close(pipe_fd[1]);
            input_fd = pipe_fd[0];
        }

        waitpid(pid, &status, 0);
    }
}

void execute_all(t_command *cmd_list) {
    t_command *cmd = cmd_list;
    int input_fd = STDIN_FILENO;

    // 시그널 무시 (Ctrl+C 시 종료 안되게)
    signal(SIGINT, handle_signal);

    while (cmd) {
        int is_last = (cmd->next == NULL);
        execute_command(cmd, input_fd, is_last);

        // 다음 파이프 연결을 위해 input_fd 갱신
        if (!is_last && cmd->outfile == NULL)
            input_fd = dup(STDIN_FILENO);  // 다음 명령어용 복사
        cmd = cmd->next;
    }
}
