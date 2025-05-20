#include "minishell.h"

ParsedCommand* parse_input(const char* input) {
    ParsedCommand* cmd = malloc(sizeof(ParsedCommand));
    initCmd(cmd);

    char* copy = strdup(input);
    char* token = strtok(copy, " ");

    ArgNode* last_node = NULL;

    while (token) {
        ArgNode* new_node = malloc(sizeof(ArgNode));

        new_node -> arg_str = strdup(token);
        new_node -> next = NULL;

        if (!(cmd -> command)) {
            cmd->command = strdup(token);
        } else {
            if (cmd -> arg_list == NULL) {
                cmd -> arg_list = new_node;
                last_node = new_node;
            } else {
                last_node -> next = new_node;
                last_node = new_node;
            }
            cmd -> argc++;
        }

        token = strtok(NULL, " ");
    }

    free(copy);
    return cmd;
}

void initCmd(ParsedCommand* P) {
    P -> arg_list = NULL;
    P -> command = NULL;
    P -> argc = 0;
}