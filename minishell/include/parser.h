#ifndef PARSER_H
# define PARSER_H

typedef struct s_command {
    char *keyword;
    char **args;
    int argc;
    char *infile;
    char *outfile;
    int background;              // 추가: 백그라운드 실행 여부
    struct s_command *next;
} t_command;


t_command *parse_input(char *input);
void print_commands(t_command *cmd);
void free_commands(t_command *cmd);

#endif
