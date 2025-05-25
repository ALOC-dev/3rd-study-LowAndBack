#include "parser.h"
#include "builtin.h"
#include <stdio.h>
#include <string.h>

typedef struct{
    char* name;
    builtin_fn fn;
}Builtin;

static Builtin builtins[] = {
    {"cd", builtin_cd},
    {"pwd", builtin_pwd},
    {"echo", builtin_echo},
    {"env", builtin_env},
    {"exit", builtin_exit},
    {"type", builtin_type}
};

int is_builtin(ParsedCommand* cmd) {
    char* name = cmd->keyword;
    for (size_t i = 0; i < sizeof(builtins)/sizeof(*builtins); i++) {
        if (strcmp(name, builtins[i].name) == 0)
            return 1;
    }
    return 0;
}

int run_builtin(ParsedCommand *cmd) {
    for (size_t i = 0; i < sizeof(builtins)/sizeof(*builtins); i++) {
        if (is_builtin(cmd)) {
            return builtins[i].fn(cmd);
        }
    }
    return 0;
}