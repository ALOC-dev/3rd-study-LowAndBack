#include "minishell.h"

int execute_builtin(ParsedCommand* cmd) {
    for (int i = 0; builtins[i].name != NULL; i++) {
        if (strcmp(cmd->command, builtins[i].name) == 0) {
            builtins[i].func(cmd);
            return 1;
        }
    }
    return 0;
}

void execute_external(ParsedCommand* cmd) {
    pid_t pid = fork();

    if (pid == 0) {
        char* argv[cmd->argc + 2];
        argv[0] = cmd->command;

        ArgNode* arg = cmd->arg_list;
        int i = 1;

        while(arg) {
            argv[i++] = arg->arg_str;
            arg = arg->next;
        }
        
        argv[i] = NULL;

        execvp(cmd->command, argv);
        fprintf(stderr, "minishell: %s: command not found\n", cmd->command);
        exit(127);
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
    } else {
        perror("fork failed");
    }
}

void execute_command(ParsedCommand* cmd) {
    if (!cmd || !cmd->command)
        return;

    if (!execute_builtin(cmd)) {
        execute_external(cmd);
    }
}