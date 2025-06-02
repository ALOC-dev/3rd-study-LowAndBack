#ifndef BUILTIN_H
# define BUILTIN_H

# include "parser.h"

// 빌트인 명령어인지 확인
int is_builtin(char *cmd);

// 빌트인 명령어 실행
void run_builtin(t_command *cmd);

// 빌트인 함수 선언
void ft_echo(t_command *cmd);
void ft_pwd(void);
void ft_cd(t_command *cmd);
void ft_exit(void);

#endif
