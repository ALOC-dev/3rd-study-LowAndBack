#ifndef PARSER_H
# define PARSER_H

typedef struct s_command {
    char *keyword;              // 첫 토큰: 명령어
    char **args;                // 나머지 인자들
    int argc;                   // 인자 개수
    char *infile;
    char *outfile;
    struct s_command *next;
} t_command;

t_command *parse_input(char *input);
void print_commands(t_command *cmd);
void free_commands(t_command *cmd);

#endif
