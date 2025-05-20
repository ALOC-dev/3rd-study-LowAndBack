#include "minishell.h"

void builtin_pwd(ParsedCommand* cmd) {
    char buf[1024];
    printf("%s\n", getcwd(buf, sizeof(buf)));
}