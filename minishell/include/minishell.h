#ifndef MINISHELL_H
# define MINISHELL_H

// 표준 헤더
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/wait.h>

// 외부 전역 환경변수
extern char **environ;

// 내부 헤더
# include "parser.h"
# include "builtin.h"
# include "executor.h"
# include "utils.h"

// REPL 함수
void repl_loop(void);

#endif
