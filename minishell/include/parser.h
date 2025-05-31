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
    char *infile;
    char *outfile;
    char *appendfile;
    struct ParsedCommand *next;
} ParsedCommand;

void add_arg(ParsedCommand *cmd, const char *arg_str);
ParsedCommand *parse_input(const char *input);
void free_parsed_command(ParsedCommand *cmd);
void print_command(ParsedCommand *cmd);

#endif // PARSER_H
