#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct ArgNode{
    char* arg;
    struct ArgNode* next;
}ArgNode;

typedef struct LinkedList{
    ArgNode* head;
    int size;
}List;

typedef struct ParsedCommand{
    char keyword;
    List args;
    int argc;
    struct ParsedCommand* next;
}ParsedCommand;


ParsedCommand* init_command(){
    ParsedCommand* command = (ParsedCommand*)malloc(sizeof(ParsedCommand));
    command->next = NULL;
    command->argc = 0;
    return command;
}

ArgNode* create_Node(char* e){
    ArgNode* node = (ArgNode*)malloc(sizeof(ArgNode));
    node->arg = strdup(e);
    node->next = NULL;
    return node;
}
int isListEmpty(List* L){
    return L->head == NULL;
}

void append_Node(List* L, char* e){
    ArgNode* node = create_Node(e);
    if (isListEmpty(L))
    {
        L->head = node;
        node->next = NULL;
        L->size++;
    } 
    else{
        ArgNode* p = L->head;
        while(p->next)
        {
            p = p->next;
        }
        p->next = node;
        node->next = NULL;
        L->size++;
    }
    return;
}

ParsedCommand* parse_input(const char* input){
    ParsedCommand* cmd = init_command();
    char* copy = strdup(input);
    char* token = strtok(copy, " ");

    if (token)
    {
        cmd->keyword = token[0];
    }
    while((token=strtok(NULL," ")) != NULL ){
        append_Node(&cmd->args, token);
        cmd->argc++;
    }
    free(copy);
    
    return cmd;
}




