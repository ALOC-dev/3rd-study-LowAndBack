#include "minishell.h"

void builtin_echo(ParsedCommand* cmd) {
    ArgNode* node = cmd->arg_list;

    while (node) {
        printf("%s", node -> arg_str);
        if (node -> next)
            printf(" ");
        node = node->next;
    }
    
    printf("\n");
}