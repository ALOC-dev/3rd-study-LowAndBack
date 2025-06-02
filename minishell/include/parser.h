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

    char *inputFile;
    char *outputFile;
    int isAppend;

    struct ParsedCommand *next;
} ParsedCommand;

ParsedCommand *parseSingleCommand(char *commandStr);
ParsedCommand *parseCommand(char *input);
void freeParsedCommand(ParsedCommand *command);
void printParsedCommand(const ParsedCommand *command);  //디버깅용

#endif