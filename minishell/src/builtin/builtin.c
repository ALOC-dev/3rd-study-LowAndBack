#include "builtin.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_builtin(char *cmd) {
    return (
        strcmp(cmd, "echo") == 0 ||
        strcmp(cmd, "pwd") == 0 ||
        strcmp(cmd, "cd") == 0 ||
        strcmp(cmd, "exit") == 0
    );
}

void run_builtin(t_command *cmd) {
    if (strcmp(cmd->keyword, "echo") == 0)
        ft_echo(cmd);
    else if (strcmp(cmd->keyword, "pwd") == 0)
        ft_pwd();
    else if (strcmp(cmd->keyword, "cd") == 0)
        ft_cd(cmd);
    else if (strcmp(cmd->keyword, "exit") == 0)
        ft_exit(cmd);
}

void ft_echo(t_command *cmd) {
    int newline = 1;
    int i = 0;

    // -n 옵션 체크
    if (cmd->argc > 0 && strcmp(cmd->args[0], "-n") == 0) {
        newline = 0;
        i = 1;
    }

    for (; i < cmd->argc; i++) {
        // 환경 변수 확장
        if (cmd->args[i][0] == '$') {
            char *val = getenv(cmd->args[i] + 1);  // $VAR → VAR
            if (val)
                printf("%s", val);
        } else {
            printf("%s", cmd->args[i]);
        }
        if (i < cmd->argc - 1)
            printf(" ");
    }
    if (newline)
        printf("\n");
}

void ft_pwd(void) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        printf("%s\n", cwd);
    else
        perror("pwd");
}

void ft_cd(t_command *cmd) {
    const char *path;
    if (cmd->argc < 1) {
        path = getenv("HOME");
        if (!path) {
            fprintf(stderr, "cd: HOME not set\n");
            return;
        }
    } else {
        path = cmd->args[0];
    }

    if (chdir(path) != 0)
        perror("cd");
}

void ft_exit(t_command *cmd) {
    int status = 0;
    if (cmd->argc >= 1)
        status = atoi(cmd->args[0]);

    printf("exit\n");
    exit(status);
}
