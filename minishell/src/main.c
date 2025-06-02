#include "minishell.h"
#include <signal.h>

void handle_sigint(int sig) {
    (void)sig;
    write(STDOUT_FILENO, "\nminishell$ ", 12);
}

void repl_loop(void) {
    char input[1024];

    while (1) {
        printf("minishell$ ");
        if (!fgets(input, sizeof(input), stdin)) {
            printf("\n");
            break;
        }

        // 비어있는 입력은 무시
        if (input[0] == '\n')
            continue;

        t_command *cmd_list = parse_input(input);
        if (!cmd_list)
            continue;

        execute_all(cmd_list);
        free_commands(cmd_list);
    }
}

int main(void) {
    // 시그널 처리: Ctrl+C 시 새 프롬프트 표시
    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, SIG_IGN);  // Ctrl+\ 무시

    repl_loop();
    return 0;
}
