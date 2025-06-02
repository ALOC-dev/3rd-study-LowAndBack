#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 문자열 공백 기준 분할
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

// 단일 명령어 세그먼트 처리
t_command *parse_segment(char *segment) {
    t_command *cmd = malloc(sizeof(t_command));
    if (!cmd) {
        perror("malloc");
        exit(1);
    }

    int argc;
    char **tokens = split_tokens(segment, &argc);
    if (argc == 0) {
        free(cmd);
        return NULL;
    }

    cmd->args = malloc(sizeof(char*) * 100);
    if (!cmd->args) {
        perror("malloc");
        exit(1);
    }

    cmd->keyword = NULL;
    cmd->argc = 0;
    cmd->infile = NULL;
    cmd->outfile = NULL;
    cmd->next = NULL;

    for (int i = 0; i < argc; i++) {
        if (strcmp(tokens[i], "<") == 0 && i + 1 < argc) {
            cmd->infile = strdup(tokens[++i]);
        } else if (strcmp(tokens[i], ">") == 0 && i + 1 < argc) {
            cmd->outfile = strdup(tokens[++i]);
        } else {
            if (!cmd->keyword) {
                cmd->keyword = strdup(tokens[i]);
            } else {
                cmd->args[cmd->argc++] = strdup(tokens[i]);
            }
        }
    }
    cmd->args[cmd->argc] = NULL;
    return cmd;
}

// 입력 문자열 전체 파싱 (파이프 구분)
t_command *parse_input(char *input) {
    t_command *head = NULL;
    t_command *curr = NULL;

    char *segment = strtok(input, "|");
    while (segment != NULL) {
        t_command *cmd = parse_segment(segment);
        if (!cmd) {
            segment = strtok(NULL, "|");
            continue;
        }

        if (!head)
            head = cmd;
        else
            curr->next = cmd;
        curr = cmd;

        segment = strtok(NULL, "|");
    }

    return head;
}

// 디버깅용 출력
void print_commands(t_command *cmd) {
    int count = 1;
    while (cmd) {
        printf("=== Command %d ===\n", count++);
        printf("Keyword: %s\n", cmd->keyword);
        for (int i = 0; i < cmd->argc; i++) {
            printf("Arg[%d]: %s\n", i, cmd->args[i]);
        }
        if (cmd->infile)
            printf("Input  : %s\n", cmd->infile);
        if (cmd->outfile)
            printf("Output : %s\n", cmd->outfile);
        cmd = cmd->next;
    }
}

// 메모리 해제
void free_commands(t_command *cmd) {
    while (cmd) {
        t_command *next = cmd->next;
        if (cmd->keyword)
            free(cmd->keyword);
        for (int i = 0; i < cmd->argc; i++)
            free(cmd->args[i]);
        free(cmd->args);
        free(cmd->infile);
        free(cmd->outfile);
        free(cmd);
        cmd = next;
    }
}
