#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
// 기본 연결리스트
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
void builtin_echo(ParsedCommand* cmd);
void builtin_pwd(ParsedCommand* cmd);
void builtin_exit(ParsedCommand* cmd);
void builtin_cd(ParsedCommand* cmd);
void builtin_type(ParsedCommand* cmd);

// executor
int execute_builtin(ParsedCommand* cmd);
void execute_command(ParsedCommand* cmd);
void execute_external(ParsedCommand* cmd);

typedef void (*BuiltinFunc)(ParsedCommand*);

typedef struct builtin_func{
    const char* name;
    BuiltinFunc func;
} b_func;

extern b_func builtins[];

#endif