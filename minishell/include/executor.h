#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"

void execute_all(t_command *cmd_list);
void execute_command(t_command *cmd);

#endif
