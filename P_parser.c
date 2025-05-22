#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_command {
    char **args;                 // 명령어 + 인자
    char *infile;               // (확장 예정) 입력 리다이렉션
    char *outfile;              // (확장 예정) 출력 리다이렉션
    struct s_command *next;     // 다음 명령어
} t_command;

// 문자열을 공백 기준으로 나누는 함수
char **split_tokens(char *line) {
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

        cmd->args = split_tokens(segment);  // 공백 기준 분할
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
    int i;
    int count = 1;
    while (cmd) {
        printf("=== Command %d ===\n", count++);
        for (i = 0; cmd->args[i]; i++) {
            printf("Arg[%d]: %s\n", i, cmd->args[i]);
        }
        cmd = cmd->next;
    }
}

// 동적 메모리 해제
void free_commands(t_command *cmd) {
    while (cmd) {
        t_command *next = cmd->next;
        free(cmd->args);  // args 내부는 strtok이 input을 쪼갠 것이므로 따로 free 안 함
        free(cmd);
        cmd = next;
    }
}

int main() {
    char input[1024];

    printf("mini-shell$ ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        perror("fgets");
        return 1;
    }

    t_command *cmd_list = parse_input(input);
    print_commands(cmd_list);

    free_commands(cmd_list);
    return 0;
}
