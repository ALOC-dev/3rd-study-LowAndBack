#include <minishell.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

void execute_commands(t_command *cmd_list) {
    t_command *cmd = cmd_list;

    while (cmd) {
        if (is_builtin(cmd->keyword)) {
            run_builtin(cmd);
        } else {
            pid_t pid = fork();
            if (pid == 0) {
                // 자식 프로세스: 외부 명령 실행
                execvp(cmd->keyword, cmd->args);
                perror("execvp");
                exit(1);
            } else if (pid > 0) {
                // 부모 프로세스: 자식 대기
                int status;
                waitpid(pid, &status, 0);
            } else {
                perror("fork");
            }
        }
        cmd = cmd->next;
    }
}
