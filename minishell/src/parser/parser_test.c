#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct ArgNode {
    char* arg_str;
    struct ArgNode* next;
} ArgNode;

typedef struct ParsedCommand {
    ArgNode* arg_list;
    char* command;
    int argc;
} ParsedCommand;

void initCmd(ParsedCommand* P) {
    P -> arg_list = NULL;
    P -> command = NULL;
    P -> argc = 0;
}

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

int main(void) {
    char input[1024];

    while(1) {
        if(fgets(input, sizeof(input), stdin) == NULL) {
            break;
        } else {
            input[strcspn(input, "\n")] = 0;

            ParsedCommand* cmd = parse_input(input);

            printf("Command: %s\n", cmd -> command);
            printf("argc: %d\n", cmd -> argc);

            ArgNode* node = cmd -> arg_list;

            while (node) {
                printf("arg: %s\n", node -> arg_str);
                node = node -> next;
            }
        }
    }
    return 0;
}