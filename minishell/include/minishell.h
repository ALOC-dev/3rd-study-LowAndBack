#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>

typedef struct ArgNode {
    char* arg_str;
    struct ArgNode* next;
} ArgNode;

typedef struct ParsedCommand {
    ArgNode* arg_list;
    char* command;
    int argc;
} ParsedCommand;


// parser
void initCmd(ParsedCommand* P);
ParsedCommand* parse_input(const char* input);

// builtin
void builtin_echo(ParsedCommand* cmd); // echo
void builtin_pwd(ParsedCommand* cmd); // pwd
void builtin_exit(ParsedCommand* cmd); // exit
void builtin_cd(ParsedCommand* cmd); // cd
void builtin_type(ParsedCommand* cmd); // type

#endif