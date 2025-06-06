#ifndef BUILTIN_H
#define BUILTIN_H

#include <stdbool.h>

bool is_builtin_command(const char *cmd);

int execute_builtin(char *const args[]);

int builtin_cd(char *const args[]);
int builtin_exit(char *const args[]);
int builtin_pwd(char *const args[]);
int builtin_echo(char *const args[]);
int builtin_type(char *const args[]);
int builtin_export(char *const args[]);

#endif // BUILTIN_H
