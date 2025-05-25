#include "minishell.h"

// 문자열을 공백 기준으로 나누는 함수
char **split_tokens(char *line, int *argc_out) {
    char **tokens = malloc(sizeof(char*) * 100);
    if (!tokens) {
        perror("malloc");
        exit(1);
    }

    char *token = strtok(line, " \t\n");
    int i = 0;
    while (token != NULL) {
        tokens[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    tokens[i] = NULL;

    *argc_out = i;
    return tokens;
}

// 파서: 파이프(|) 기준으로 명령어 단위로 나눈 뒤, 연결 리스트 생성
t_command *parse_input(char *input) {
    t_command *head = NULL;
    t_command *curr = NULL;

    char *segment = strtok(input, "|");
    while (segment != NULL) {
        t_command *cmd = malloc(sizeof(t_command));
        if (!cmd) {
            perror("malloc");
            exit(1);
        }

        int argc;
        char **tokens = split_tokens(segment, &argc);

        if (argc == 0) {
            free(cmd);
            segment = strtok(NULL, "|");
            continue;
        }

        cmd->keyword = tokens[0];              // 첫 번째 토큰 → keyword
        cmd->argc = argc - 1;                  // 나머지 토큰 개수
        cmd->args = malloc(sizeof(char*) * (cmd->argc + 1));
        if (!cmd->args) {
            perror("malloc");
            exit(1);
        }

        for (int i = 1; i < argc; i++)
            cmd->args[i - 1] = tokens[i];
        cmd->args[cmd->argc] = NULL;

        cmd->infile = NULL;
        cmd->outfile = NULL;
        cmd->next = NULL;

        if (head == NULL)
            head = cmd;
        else
            curr->next = cmd;

        curr = cmd;
        segment = strtok(NULL, "|");
    }

    return head;
}

// 파싱된 명령어들 출력
void print_commands(t_command *cmd) {
    int count = 1;
    while (cmd) {
        printf("=== Command %d ===\n", count++);
        printf("Keyword: %s\n", cmd->keyword);
        for (int i = 0; i < cmd->argc; i++) {
            printf("Arg[%d]: %s\n", i, cmd->args[i]);
        }
        cmd = cmd->next;
    }
}

// 동적 메모리 해제
void free_commands(t_command *cmd) {
    while (cmd) {
        t_command *next = cmd->next;
        free(cmd->args);      // args 자체만 free (내부는 원래 문자열 토큰)
        free(cmd);            // 구조체 free
        cmd = next;
    }
}
