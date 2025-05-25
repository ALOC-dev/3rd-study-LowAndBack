#include "minishell.h"

void builtin_pwd(ParsedCommand* cmd) {
    (void) cmd;
    char buf[1024];
    printf("%s\n", getcwd(buf, sizeof(buf)));
}