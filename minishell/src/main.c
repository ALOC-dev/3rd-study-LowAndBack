#include "minishell.h"

void repl_loop(void) {
    char input[1024];

    while (1) {
        printf("minishell$ ");
        if (!fgets(input, sizeof(input), stdin)) {
            printf("\n");
            break;
        }

        t_command *cmd_list = parse_input(input);
        t_command *curr = cmd_list;

        while (curr) {
            if (curr->keyword == NULL) {
                curr = curr->next;
                continue;
            }

            if (is_builtin(curr->keyword))
                run_builtin(curr);
            else
                printf("외부 명령 실행: %s (여기서는 처리 안 함)\n", curr->keyword);

            curr = curr->next;
        }

        free_commands(cmd_list);
    }
}

int main(void) {
    repl_loop();
    return 0;
}
