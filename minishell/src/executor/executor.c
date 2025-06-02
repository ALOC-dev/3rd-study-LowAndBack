#include "executor.h"
#include "builtin.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

void execute_command(t_command *cmd) {
    pid_t pid;
    int status;

    pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        // 자식 프로세스

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

        // 3. builtin 명령어 처리
        if (is_builtin(cmd->keyword)) {
            run_builtin(cmd);
            exit(0); // 자식 프로세스에서 실행 후 종료
        }

        // 4. 외부 명령어 실행
        execvp(cmd->keyword, cmd->args);
        perror("execvp"); // exec 실패 시
        exit(1);
    } else {
        // 부모 프로세스
        waitpid(pid, &status, 0);
    }
}

void execute_all(t_command *cmd_list) {
    t_command *cmd = cmd_list;
    while (cmd) {
        execute_command(cmd);
        cmd = cmd->next;
    }
}
