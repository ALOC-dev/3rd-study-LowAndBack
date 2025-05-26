#include "builtin.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


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
        ft_exit();
}

void ft_echo(t_command *cmd) {
    for (int i = 0; i < cmd->argc; i++) {
        printf("%s", cmd->args[i]);
        if (i < cmd->argc - 1)
            printf(" ");
    }
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
    if (cmd->argc < 1) {
        fprintf(stderr, "cd: missing argument\n");
        return;
    }
    if (chdir(cmd->args[0]) != 0)
        perror("cd");
}

void ft_exit(void) {
    printf("exit\n");
    exit(0);
}
