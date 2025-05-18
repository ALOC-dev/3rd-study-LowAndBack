#ifndef PARSER_H
#define PARSER_H

typedef struct ArgNode {
    char *arg;
    struct ArgNode *next;
} ArgNode;

typedef struct ParsedCommand {
    char *keyword;
    ArgNode *args;
    int argc;
    struct ParsedCommand *next; // 명령어가 여러 줄 들어오거나, pipe가 있을 때 사용
} ParsedCommand;

ParsedCommand *parseCommand(char *input);
void freeParsedCommand(ParsedCommand *command);
void printParsedCommand(const ParsedCommand *command);  //디버깅용

#endif