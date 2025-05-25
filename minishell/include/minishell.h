#ifndef MINISHELL_H
#define MINISHELL_H

// 구조체 정의
typedef struct s_command {
    char *keyword;              // 첫 번째 명령어 (기존 args[0])
    char **args;                // 나머지 인자들 (args[0]부터)
    int argc;                   // 인자 개수
    char *infile;
    char *outfile;
    struct s_command *next;
} t_command;

// 함수 선언
t_command *parse_input(char *input);
void print_commands(t_command *cmd);
void free_commands(t_command *cmd);

#endif
