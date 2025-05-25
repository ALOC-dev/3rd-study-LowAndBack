#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parser.h"

int builtin_echo(List* L){
    ArgNode* p = L->head;                                           // 리스트의 head를 가르키는 노드 포인터 선언
    if (!p)                                                         // 만약 리스트가 비어있다면(NULL) 
    {
        putchar('\n');                                              // 개행문자 출력 후 종료
        return 0;
    }
    bool no_newline = ((strcmp("-n" , p->arg) == 0));               // 리스트의 head에 -n이 있다면 true
    if (no_newline)                                                 // -n이 있다면
    {
        p = p->next;                                                // -n 다음 인자로 포인터를 옮김
    }
    bool first = true;
    for (; p ; p = p->next)                        // p가 NULL이기 전까지 반복, 한 번 사이클이 끝나면 p를 다음 인자로 포인터를 옮김
    {
        if (!first)                                                 // 만약 첫 번째 인자가 아니라면 ' ' 출력
        {
            putchar(' ');
        }
        first = false;                                              // 이제 첫 번째 인자가 아니므로 false로 변경
        fputs(p->arg, stdout);                                      // 인자를 출력
    }
    if (!no_newline){                                               // 만약 -n이 입력되지 않았다면
        putchar('\n');                                              // 개행문자 출력
    }
    return 0;                                                       // 종료
}