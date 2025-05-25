#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "builtin.h"

int builtin_pwd(ParsedCommand* cmd){
    char* cwd = getcwd(NULL,0);                     // 현재 디렉토리를 getcwd로 구하고, ()안에 NULL,0으로 필요한 문자열의 크기만큼 동적 할당을 해줌
    if (cwd != NULL)                                // cwd가 비어있지 않다면 현재 디렉토리를 출력 후 메모리 해제
    {
        printf("%s\n", cwd);
        free(cwd);
    }
    
    return 0;
}