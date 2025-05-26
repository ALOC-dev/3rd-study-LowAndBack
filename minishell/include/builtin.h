#ifndef BUILTIN_H
# define BUILTIN_H

# include "parser.h"

int is_builtin(char *cmd);
void run_builtin(t_command *cmd);

// 각각의 함수들
void ft_echo(t_command *cmd);
void ft_pwd(void);
void ft_cd(t_command *cmd);
void ft_exit(void);

#endif
