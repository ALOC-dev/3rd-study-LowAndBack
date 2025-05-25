#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

ParsedCommand* init_command(){
    ParsedCommand* command = (ParsedCommand*)malloc(sizeof(ParsedCommand));
    command->next = NULL;
    command->argc = 0;
    command->args = NULL;
    command -> keyword = NULL;
    return command;
}

ArgNode* create_Node(char* e){
    ArgNode* node = (ArgNode*)malloc(sizeof(ArgNode));
    node->arg = strdup(e);
    node->next = NULL;
    return node;
}
int isListEmpty(ParsedCommand* cmd){
    return cmd->args == NULL;
}

void append_Node(ParsedCommand* cmd, char* e){
    ArgNode* node = create_Node(e);
    if (isListEmpty(cmd))
    {
        cmd->args = node;
        node->next = NULL;
        cmd->argc++;
    } 
    else{
        ArgNode* p = cmd->args;
        while(p->next != NULL)
        {
            p = p->next;
        }
        p->next = node;
        node->next = NULL;
        cmd->argc++;
    }
    return;
}

ParsedCommand* parse_input(const char* input){
    ParsedCommand* cmd = init_command();
    char* copy = strdup(input);
    char* token = strtok(copy, " ");

    if (token)
    {
        cmd->keyword = strdup(token);
    }
    while((token=strtok(NULL," ")) != NULL ){
        append_Node(cmd, token);
    }
    free(copy);
    
    return cmd;
}

void print_parsed_command(const ParsedCommand* cmd){
    if (cmd == NULL)
    {
        printf("cmd is empty\n");
        return;
    }
    printf("keyword: %s\n", cmd->keyword);
    printf("argument: ");
    ArgNode* p = cmd->args;

    while (p)
    {
        printf("%s ", p->arg);
        p = p->next;
    }
    printf("\n");
    return; 
}

void free_parsed_command(const ParsedCommand* cmd){
    if (cmd == NULL)
    {
        return;
    }
    ArgNode* current = cmd->args;
    while (current != NULL)
    {
        ArgNode* next = current->next;
        free(current->arg);
        free(current);
        current = next;
    }
    free(cmd->keyword);
    free(cmd);
    return;
}
