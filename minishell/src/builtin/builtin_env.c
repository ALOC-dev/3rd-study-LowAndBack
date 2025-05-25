#include <stdio.h>
#include "builtin.h"

int builtin_env(ParsedCommand* cmd){
    extern char **environ;                          
    for (int i = 0; environ[i] != NULL; i++)
    {
        printf("%s" , environ[i]);    
    }
    return 0;
}
