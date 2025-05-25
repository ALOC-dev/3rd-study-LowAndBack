#include <stdio.h>

int builtin_env(){
    extern char **environ;                          
    for (int i = 0; environ[i] != NULL; i++)
    {
        printf("%s" , environ[i]);    
    }
    return 0;
}
