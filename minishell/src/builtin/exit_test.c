#include "minishell.h"

void builtin_exit(ParsedCommand* cmd) {
    // if (strcmp(cmd->command, "exit") != 0)
    //     return;

    ArgNode* arg = cmd -> arg_list;

    if (cmd -> argc == 0) {
        exit(0);
    } else if (cmd -> argc == 1) {
        char* s = arg -> arg_str;
        int is_num = 1;

        for (int i = 0; s[i]; i++) {
            if (!isdigit(s[i])) {
                is_num = 0;
                break;
            }
        }

        if(!is_num) {
            printf("minishell: exit: %s: numeric argument required\n", s);
            return;
        } else {
            int code = atoi(s);
            exit(code);
        }

    } else {
        printf("minishell: exit: too many arguments\n");
    }
}