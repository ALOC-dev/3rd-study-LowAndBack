#include "minishell.h"

int main(void) {
    char input[1024];

    while (1) {
        char route[1024];
        printf("minishell [%s] >> ", getcwd(route, sizeof(route)));

        if (fgets(input, sizeof(input), stdin) == NULL)
            break;

        input[strcspn(input, "\n")] = 0;

        ParsedCommand* cmd = parse_input(input);

        execute_builtin(cmd);

        // if (strcmp(cmd->command, "echo") == 0) {
        //     builtin_echo(cmd);
        // } else if (strcmp(cmd->command, "pwd") == 0) {
        //     builtin_pwd(cmd);
        // } else if (strcmp(cmd->command, "exit") == 0) {
        //     builtin_exit(cmd);
        // } else if (strcmp(cmd->command, "cd") == 0) {
        //     builtin_cd(cmd);
        // } else {
        //     printf("minishell: command not found: %s\n", cmd->command);
        // }
    }

    return 0;
}