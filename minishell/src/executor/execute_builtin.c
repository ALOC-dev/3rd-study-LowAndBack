#include "minishell.h"

b_func builtins[] = {
        { "echo", builtin_echo },
        { "pwd", builtin_pwd },
        { "cd",  builtin_cd },
        { "exit", builtin_exit },
        { "type", builtin_type },
        { NULL, NULL }
    };

void execute_builtin(ParsedCommand* cmd) {
    for (int i = 0; builtins[i].name != NULL; i++) {
        if (strcmp(cmd->command, builtins[i].name) == 0) {
            return builtins[i].func(cmd);
        }
    }
    return;
}