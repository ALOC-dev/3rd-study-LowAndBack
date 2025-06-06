#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>   
#include "parser.h"

void add_arg(ParsedCommand *cmd, const char *arg_str) {
    ArgNode *new_node = malloc(sizeof(ArgNode));
    if (arg_str[0] == '$') {
        char *env_val = getenv(arg_str + 1); // 환경변수 처리
        new_node->arg = env_val ? strdup(env_val) : strdup("");
    } else {
        new_node->arg = strdup(arg_str);
    }
    new_node->next = NULL;

    if (cmd->args == NULL) {
        cmd->args = new_node;
    } else {
        ArgNode *cur = cmd->args;
        while (cur->next != NULL)
            cur = cur->next;
        cur->next = new_node;
    }
    cmd->argc++;
}

void free_parsed_command(ParsedCommand *cmd) {
    while (cmd) {
        ParsedCommand *next_cmd = cmd->next;
        ArgNode *cur = cmd->args;
        while (cur) {
            ArgNode *next = cur->next;
            free(cur->arg);
            free(cur);
            cur = next;
        }
        free(cmd->keyword);
        free(cmd->infile);
        free(cmd->outfile);
        free(cmd->appendfile);
        free(cmd);
        cmd = next_cmd;
    }
}

ParsedCommand *parse_input(const char *input) {
    char *input_copy = strdup(input);
    char *token = strtok(input_copy, " ");
    ParsedCommand *head = NULL, *current = NULL;

    while (token != NULL) {
        ParsedCommand *cmd = calloc(1, sizeof(ParsedCommand));
        cmd->argc = 0;
        cmd->args = NULL;
        cmd->infile = NULL;
        cmd->outfile = NULL;
        cmd->appendfile = NULL;
        cmd->is_background = 0; //백그라운드 실행 여부
        cmd->next = NULL;

        if (token == NULL) break;
        cmd->keyword = strdup(token);
        add_arg(cmd, token);

        while ((token = strtok(NULL, " ")) != NULL) {
            if (strcmp(token, "|") == 0) {
                token = strtok(NULL, " ");
                break;
            } else if (strcmp(token, "<") == 0) {
                token = strtok(NULL, " ");
                if (token) cmd->infile = strdup(token);
            } else if (strcmp(token, ">>") == 0) {
                token = strtok(NULL, " ");
                if (token) cmd->appendfile = strdup(token);
            } else if (strcmp(token, ">") == 0) {
                token = strtok(NULL, " ");
                if (token) cmd->outfile = strdup(token);
            } else if (strcmp(token, "&") == 0) {
                cmd->is_background = 1; // <- '&' 감지 시 백그라운드 플래그 설정
            } else {
                add_arg(cmd, token);
            }
        }

        if (head == NULL) {
            head = cmd;
        } else {
            current->next = cmd;
        }
        current = cmd;
    }

    free(input_copy);
    return head;
}

void print_command(ParsedCommand *cmd) {
    int i = 1;
    while (cmd) {
        printf("[Command %d]\n", i++);
        printf("  Keyword: %s\n", cmd->keyword);
        printf("  Args (%d):", cmd->argc);
        ArgNode *cur = cmd->args;
        while (cur) {
            printf(" %s", cur->arg);
            cur = cur->next;
        }
        printf("\n");
        if (cmd->infile)     printf("  Input Redirect: %s\n", cmd->infile);
        if (cmd->outfile)    printf("  Output Redirect: %s\n", cmd->outfile);
        if (cmd->appendfile) printf("  Append Redirect: %s\n", cmd->appendfile);
        if (cmd->is_background) printf("  Background Execution: YES\n");
        cmd = cmd->next;
    }
}
