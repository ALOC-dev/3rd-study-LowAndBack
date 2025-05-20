#include "minishell.h"

int main(void) {
    char input[1024];

    while (1) {
        printf("minishell >> ");

        if (fgets(input, sizeof(input), stdin) == NULL)
            break;

        input[strcspn(input, "\n")] = 0;

        ParsedCommand* cmd = parse_input(input);

        if (strcmp(cmd->command, "echo") == 0) {
            builtin_echo(cmd);
        } else if (strcmp(cmd->command, "pwd") == 0) {
            builtin_pwd(cmd);
        } else {
            printf("unknown command: %s\n", cmd->command);
        }
    }

    return 0;
}