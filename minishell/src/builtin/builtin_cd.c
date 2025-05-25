#include "minishell.h"

void builtin_cd(ParsedCommand* cmd) {
    if (cmd->argc == 0) {
        char* home = getenv("HOME");
        chdir(home);
    } else if (cmd->argc == 1) {
        const char* path = cmd -> arg_list -> arg_str;
        if (chdir(path) == -1) {
            perror("minishell: cd");
        }
    } else if (cmd->argc > 1) {
        printf("minishell: cd: too many arguments\n");
    }
}