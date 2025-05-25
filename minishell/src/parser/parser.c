#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_command {
    char *keyword;              // 명령어 (첫 번째 토큰)
    char **args;                // 인자 목록 (두 번째부터)
    int argc;                   // 인자 개수
    char *infile;              // (확장 예정) 입력 리다이렉션
    char *outfile;             // (확장 예정) 출력 리다이렉션
    struct s_command *next;     // 다음 명령어
} t_command;

// 입력 문자열을 파싱해서 keyword, args, argc 분리
void parse_tokens(char *line, t_command *cmd) {
    char *tokens[100];
    int i = 0;

    char *token = strtok(line, " \t\n");
    while (token && i < 100) {
        tokens[i++] = token;
        token = strtok(NULL, " \t\n");
    }

    if (i == 0) {
        cmd->keyword = NULL;
        cmd->args = NULL;
        cmd->argc = 0;
        return;
    }

    cmd->keyword = strdup(tokens[0]);  // 첫 번째 토큰은 keyword
    cmd->argc = i - 1;                 // 나머지는 args
    cmd->args = malloc(sizeof(char*) * (cmd->argc + 1));
    if (!cmd->args) {
        perror("malloc");
        exit(1);
    }

    for (int j = 0; j < cmd->argc; j++) {
        cmd->args[j] = strdup(tokens[j + 1]);
    }
    cmd->args[cmd->argc] = NULL;
}

// 파이프 단위로 명령어 나누고 연결 리스트 구성
t_command *parse_input(char *input) {
    t_command *head = NULL;
    t_command *curr = NULL;

    char *segment = strtok(input, "|");
    while (segment) {
        t_command *cmd = malloc(sizeof(t_command));
        if (!cmd) {
            perror("malloc");
            exit(1);
        }

        cmd->infile = NULL;
        cmd->outfile = NULL;
        cmd->next = NULL;

        parse_tokens(segment, cmd);

        if (!head)
            head = cmd;
        else
            curr->next = cmd;

        curr = cmd;
        segment = strtok(NULL, "|");
    }

    return head;
}

// 디버깅용: 파싱 결과 출력
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

        free(cmd->keyword);
        for (int i = 0; i < cmd->argc; i++) {
            free(cmd->args[i]);
        }
        free(cmd->args);
        free(cmd);

        cmd = next;
    }
}

int main() {
    char input[1024];

    while (1) {
        printf("mini-shell$ ");
        if (!fgets(input, sizeof(input), stdin)) {
            printf("\n");
            break;
        }

        if (input[0] == '\n') continue;

        t_command *cmd_list = parse_input(input);
        print_commands(cmd_list);  // 임시 디버깅용

        // TODO: execute_commands(cmd_list); 추가 예정

        free_commands(cmd_list);
    }

    return 0;
}
