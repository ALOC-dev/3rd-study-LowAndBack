#include "minishell.h"

void builtin_type(ParsedCommand* cmd) {
    ArgNode* arg = cmd -> arg_list;

    while(arg) {
        int found = 0;

        for (int i = 0; builtins[i].name != NULL; i++) {
            if (strcmp(arg -> arg_str, builtins[i].name) == 0) {
                printf("%s is a shell builtin\n", arg->arg_str);
                found = 1;
                break;
            }
        }

        if(!found) {
            printf("%s is not found\n", arg->arg_str);
        }

        arg = arg -> next;
    }
}