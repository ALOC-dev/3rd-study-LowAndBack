#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "parser.h"

int builtin_cd(ParsedCommand* cmd){                    // 리스트를 매개변수로 입력받음
    ArgNode* p = cmd->args;                   // 리스트의 head를 가르키는 포인터 선언
    if (!p)                                 // 만약 head가 비어있다면
    {
        char* home = getenv("HOME");        // getenv로 홈 디렉토리를 home 문자열에 저장
        chdir(home);                        // chdir(home)으로 디렉토리를 home으로 변경
        return 0;
    }
    chdir(p->arg);                          // head가 비어있지 않다면 해당 디렉토리로 변경

    return 0;
}