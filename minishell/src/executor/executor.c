// executor.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

void execute_command(char *const args[]) {
    // 명령어가 비어 있는 경우
    if (args == NULL || args[0] == NULL) {
        fprintf(stderr, "Error : no command provided \n");
        return;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return;
    } else if (pid == 0) {
        execvp(args[0], args);

        // execvp가 반환되었다면 실패한 것임
        perror("execvp failed");
        exit(EXIT_FAILURE);
    } else {
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid failed");
            return;
        }

        // 자식 프로세스가 정상 종료되었는지 확인
        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            if (exit_status != 0) {
                fprintf(stderr, "Command exited with status %d\n", exit_status);
            }
        } else if (WIFSIGNALED(status)) {
            fprintf(stderr, "Command terminated by signal %d\n", WTERMSIG(status));
        }
    }
}
