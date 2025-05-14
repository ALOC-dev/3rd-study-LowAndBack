#ifndef parser_H
#define parser_H

typedef struct {
  char *keyword;
  char **args;
  int argc;
} ParsedCommand;

ParsedCommand *parseCommand(const char *input);

#endif