#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "parser.h"

int is_number_str(const char* a){
    if (a[0] == '\0')
    {
        return 0;
    }
    int i = 0;
    if (a[0] == '-' || a[0] == '+')
    {
        i++;
    }
    for ( ; a[i]!='\0' ; i++)
    {
        if (!isdigit(a[i]))
        {
            return 0;
        }
    }
    return 1;   
}
int builtin_exit(ParsedCommand* cmd){
    ArgNode* p = cmd->args;
    if (cmd->argc == 0){
        exit(0);
    }

    if (cmd->argc>1){
        fprintf(stderr, "too many arguments\n");
        return 0;
    }

    if (is_number_str(p->arg))
    {
        exit(atoi(p->arg)); 
    } else{
        fprintf(stderr, "numeric argument required\n");
        exit(255);
    }

    return 0;
}
