#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <unistd.h>
# include <signal.h>

// 단일 명령 실행
void execute_command(t_command *cmd, int input_fd, int output_fd, int is_background);

// 전체 파이프라인 실행
void execute_all(t_command *cmd_list);

// 파일 디스크립터 설정
int setup_redirection(t_command *cmd);

// 시그널 처리기 설정
void set_signal_handlers(void);

// 백그라운드 실행 여부 판단
int is_background_command(t_command *cmd);

#endif
