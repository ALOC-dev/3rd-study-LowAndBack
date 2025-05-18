#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ArgNode {
    char *arg;
    struct ArgNode *next;
} ArgNode;

// 파싱된 명령어를 담는 구조체
typedef struct ParsedCommand {
    char *keyword;      // 첫 단어 (명령어)
    ArgNode *args;      // 인자들 연결 리스트
    int argc;           // 인자 개수
} ParsedCommand;

// 인자 리스트에 새 인자 추가
void add_arg(ParsedCommand *cmd, const char *arg_str) {
    ArgNode *new_node = malloc(sizeof(ArgNode));
    new_node->arg = strdup(arg_str);
    new_node->next = NULL;

    if (cmd->args == NULL) {
        cmd->args = new_node;
    } else {
        ArgNode *cur = cmd->args;
        while (cur->next != NULL)
            cur = cur->next;
        cur->next = new_node;
    } cmd->argc++;
}

// 입력 문자열을 구조체로 변환
ParsedCommand *parse_input(const char *input) {
    char *input_copy = strdup(input);
    char *token = strtok(input_copy, " ");

    if (token == NULL) {
        free(input_copy);
        return NULL;
    }

    // 구조체 초기화
    ParsedCommand *cmd = malloc(sizeof(ParsedCommand));
    cmd->keyword = strdup(token);
    cmd->args = NULL;
    cmd->argc = 0;

    // 나머지 토큰은 인자로 처리
    while ((token = strtok(NULL, " ")) != NULL) {
        add_arg(cmd, token);
    }

    free(input_copy);
    return cmd;
}

// 결과 출력
void print_command(ParsedCommand *cmd) {
    if (cmd == NULL) {
        printf("No command parsed.\n");
        return;
    }

    printf("Keyword: %s\n", cmd->keyword);
    printf("Arg count: %d\n", cmd->argc);

    ArgNode *cur = cmd->args;
    while (cur != NULL) {
        printf("Arg: %s\n", cur->arg);
        cur = cur->next;
    }
}

int main() {
    char input[100];
    printf("put >");
    fgets(input, sizeof(input), stdin);

    input[strcspn(input, "\n")] = '\0';     //줄바꿈 문자 X

    ParsedCommand *cmd = parse_input(input);
    print_command(cmd);

    return 0;
}
