#ifndef BUILTIN_H
#define BUILTIN_H

#include <stdio.h>
#include "parser.h"
#include "builtin.h"

typedef int (*builtin_fn)(ParsedCommand *cmd);

int builtin_echo(ParsedCommand* cmd);
int builtin_pwd(ParsedCommand* cmd);
int builtin_cd(ParsedCommand* cmd);
int builtin_env(ParsedCommand* cmd);
int builtin_exit(ParsedCommand* cmd);
int builtin_type(ParsedCommand* cmd);

int is_builtin(ParsedCommand* cmd);
int run_builtin(ParsedCommand* cmd);

#endif