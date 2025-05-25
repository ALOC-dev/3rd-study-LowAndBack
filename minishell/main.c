#include "minishell.h"

b_func builtins[] = {
        { "echo", builtin_echo },
        { "pwd", builtin_pwd },
        { "cd",  builtin_cd },
        { "exit", builtin_exit },
        { "type", builtin_type },
        { NULL, NULL }
};

int main(void) {
    char input[1024];

    while (1) {
        char route[1024];
        printf("minishell [%s] >> ", getcwd(route, sizeof(route)));

        if (fgets(input, sizeof(input), stdin) == NULL)
            break;

        input[strcspn(input, "\n")] = 0;

        ParsedCommand* cmd = parse_input(input);

        execute_command(cmd);
    }
    return 0;
}