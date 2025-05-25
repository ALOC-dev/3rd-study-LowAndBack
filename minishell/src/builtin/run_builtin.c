#include "parser.h"
#include "builtin.h"
#include <stdio.h>
#include <string.h>

typedef void (*builtin_fn)(List*);
typedef struct{
    char* name;
    builtin_fn fn;
}Builtin;

Builtin builtins[] = {
    {"cd", (builtin_fn)builtin_cd},
    {"pwd", (builtin_fn)builtin_pwd},
    {"echo", (builtin_fn)builtin_echo},
    {"env", (builtin_fn)builtin_env},
    {"exit", (builtin_fn)builtin_exit},
    {"type", (builtin_fn)builtin_type}
};

int run_builtin(ParsedCommand* cmd){
    size_t cnt = sizeof(builtins) / sizeof(builtins[0]);

    for (size_t i = 0; i < cnt; i++)
    {
        if (strcmp(cmd->keyword,builtins[i].name) == 0)
        {
            builtins[i].fn(&cmd->args);
            return 1;
        }
    }
    return 0;
}