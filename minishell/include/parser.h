// parser.h
#ifndef PARSER_H
#define PARSER_H

typedef struct ArgNode {
    char* arg;
    struct ArgNode* next;
} ArgNode;


typedef struct LinkedList {
    ArgNode* head;
    int size;
} List;


typedef struct ParsedCommand {
    char* keyword;
    List* args;
    int argc;
    struct ParsedCommand* next;
} ParsedCommand;

// 함수 선언
ParsedCommand* init_command();
ArgNode* create_Node(char* e);
int isListEmpty(List* L);
void append_Node(List* L, char* e);
ParsedCommand* parse_input(const char* input);

#endif