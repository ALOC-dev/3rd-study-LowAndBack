#include "minishell.h"

void builtin_pwd(ParsedCommand* cmd) {
    if(strcmp(cmd -> command, "pwd") == 0) {
        char buf[1024];
        printf("%s\n", getcwd(buf, sizeof(buf)));
    }
}