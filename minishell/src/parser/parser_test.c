#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct ArgNode {
    char* arg_str;
    struct ArgNode* next;
} ArgNode;

typedef struct ParsedCommand {
    ArgNode* arg_list;
    char* command;
    int argc;
    struct ParsedCommand* command_list;
} ParsedCommand;

typedef struct CommandList {
    ParsedCommand* cmd_list;
    int command_count;
} cmdL;

void initCmd(ParsedCommand* P) {
    P -> arg_list = NULL;
    P -> command = NULL;
    P -> argc = 0;
    P -> command_list = NULL;
}

ParsedCommand* parse_input(const char* input) {
    ParsedCommand* cmd = malloc(sizeof(ParsedCommand));
    ArgNode* new_node = malloc(sizeof(ArgNode));
    ArgNode* last_node = NULL;

    initCmd(cmd);

    char* copy = strdup(input); // 복사
    char* token = strtok(copy, " "); // 공백 기준으로 문자열 분리

    while (token) {
        if (!(cmd->command)) { // 연결리스트가 비어있다면 true (첫번째 노드라면)
            cmd -> command = strdup(token); // 첫번째 단어이므로 명령어에 저장
        } else {
            new_node -> arg_str = strdup(token); // token으로 분리된 단어 저장
            new_node -> next = NULL;

            if(last_node == NULL) {
                last_node = new_node;
            } else {
                last_node -> next = new_node; // 첫번째 노드가 아니라면 이전 node와 현재 노드와 연결
                last_node = new_node; // 다음 들어올 노드와 이어주기 위해 last_node로 참조
            }
            cmd -> argc++;
        }
        token = strtok(NULL, " "); // NULL을 넣어주면 이미 분리된 문자열 그대로 사용함
    }

    free(copy);

    return cmd;
}

int main(void) {
    char input[1024];

    while(1) {
        if(fgets(input, sizeof(input), stdin) == NULL) {
            break;
        } else {
            input[strcspn(input, "\n")] = 0;

            ParsedCommand* cmd = parse_input(input);

            printf("Command: %s\n", cmd -> command);
            printf("argc: %d\n", cmd -> argc);

            ArgNode* node = cmd -> arg_list;
            
            while (node) {
                printf("arg: %s\n", node -> arg_str);
                node = node -> next;
            }
        }
    }
    return 0;
}