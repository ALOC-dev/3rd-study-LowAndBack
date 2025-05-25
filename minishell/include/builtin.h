#ifndef BUILTIN_H
#define BUILTIN_H

#include <stdio.h>
#include "parser.h"
#include "builtin.h"

int is_builtin(ParsedCommand* cmd);
int builtin_echo(List* L);
int builtin_pwd();
int builtin_cd(List* L);
int builtin_env();
void builtin_exit(List* L);
int run_builtin(ParsedCommand* cmd);
void builtin_type(List* L);

#endif