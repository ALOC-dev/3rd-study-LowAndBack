#include <stdio.h>
#include <string.h>
#include "parser.h"


int is_builtin(ParsedCommand* cmd){
    char* kw = cmd->keyword;
    if (!kw)
    {
        return 0;
    }
    return (!strcmp(kw,"cd")||
            !strcmp(kw,"echo")||
            !strcmp(kw,"env")||
            !strcmp(kw,"exit")||
            !strcmp(kw,"pwd")||
            !strcmp(kw,"type"));
    
}