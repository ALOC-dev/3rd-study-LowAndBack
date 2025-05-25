#include <stdio.h>
#include "parser.h"
#include "builtin.h"

void builtin_type(List* L){
    ArgNode* p = L->head;
    if (is_builtin(L))
    {
        printf("%s is a shell builtin", p->arg);
        return;
    }
    
    
    
}