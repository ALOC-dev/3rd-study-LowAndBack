#include "minishell.h"

void builtin_cd(ParsedCommand* cmd) {
    ArgNode* arg = cmd -> arg_list;
    // 인자 1개면 그 경로로 이동
    if(cmd -> argc == 0) {
        char* home = getenv("HOME");
        chdir(home);
        printf("%s\n", home);
    } else if(cmd -> argc == 1) {

    } else if(cmd -> argc > 1) {
        printf("minishell: cd: too many arguments\n");        
    } else {
        printf("minishell: cd: invalid arguments\n");
        return;
    }
}