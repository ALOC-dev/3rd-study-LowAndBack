#include "minishell.h"
#include <stdio.h>

int main(void) {
    char input[1024];
    printf("mini-shell$ ");
    if (fgets(input, sizeof(input), stdin) == NULL)
        return 1;

    t_command *cmd_list = parse_input(input);
    print_commands(cmd_list);
    free_commands(cmd_list);
    return 0;
}
