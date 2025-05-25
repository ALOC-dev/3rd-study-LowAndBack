#include <stdio.h>
#include "parser.h"
#include "builtin.h"

int builtin_type(ParsedCommand* cmd){
    if (is_builtin(cmd->keyword))
    {
        printf("%s is a shell builtin", cmd->keyword);
        return 0;
    }
    
}