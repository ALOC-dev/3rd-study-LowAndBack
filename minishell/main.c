#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <parser.h>
#include <executor.h>

int main() {
    char input[1024];

    while (1) {
        printf("minishell> ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\n");
            break;
        }

        input[strcspn(input, "\n")] = '\0';
        if (strlen(input) == 0)
            continue;

        ParsedCommand *cmd = parse_input(input);
        if (!cmd) {
            fprintf(stderr, "Parsing failed\n");
            continue;
        }

        int status = execute_command(cmd);
        free_parsed_command(cmd);

        if (status == -1) // exit 명령어
            break;
    }

    return 0;
}
