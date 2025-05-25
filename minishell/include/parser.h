// parser.h
#ifndef PARSER_H
#define PARSER_H

typedef struct ArgNode {
    char* arg;
    struct ArgNode* next;
} ArgNode;


typedef struct ParsedCommand {
    char* keyword;
    ArgNode* args;
    int argc;
    struct ParsedCommand* next;
} ParsedCommand;

// 함수 선언
ParsedCommand* init_command();
ArgNode* create_Node(char* e);
int isListEmpty(ParsedCommand* cmd);
void append_Node(ParsedCommand* cmd, char* e);
ParsedCommand* parse_input(const char* input);
void print_parsed_command(const ParsedCommand* cmd);
void free_parsed_command(const ParsedCommand* cmd);

#endif