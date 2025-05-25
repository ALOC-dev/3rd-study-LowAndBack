#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

typedef struct s_command {
    char *keyword;              // 첫 토큰: 명령어
    char **args;                // 나머지 인자들
    int argc;                   // 인자 개수
    char *infile;
    char *outfile;
    struct s_command *next;
} t_command;

// parser
t_command *parse_input(char *input);
void print_commands(t_command *cmd);
void free_commands(t_command *cmd);

// builtin
int is_builtin(char *cmd);
void run_builtin(t_command *cmd);

// 각각의 함수들
void ft_echo(t_command *cmd);
void ft_pwd(void);
void ft_cd(t_command *cmd);
void ft_exit(void);

#endif
