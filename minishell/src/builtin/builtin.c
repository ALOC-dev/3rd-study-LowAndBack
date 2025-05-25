#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT 1024

// 내장 명령어 함수 선언
void shell_echo(char **args);
void shell_pwd(char **args);
void shell_cd(char **args);
void shell_exit(char **args);
void shell_type(char **args);

// 명령어 이름들과 함수들 연결
char *builtin_str[] = {
    "echo",
    "pwd",
    "cd",
    "exit",
    "type"
};

void (*builtin_func[])(char **) = {
    &shell_echo,
    &shell_pwd,
    &shell_cd,
    &shell_exit,
    &shell_type
};

int num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

// 각 내장 명령어 구현

void shell_echo(char **args) {
    for (int i = 1; args[i] != NULL; i++) {
        printf("%s ", args[i]);
    }
    printf("\n");
}

void shell_pwd(char **args) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("pwd");
    }
}

void shell_cd(char **args) {
    if (args[1] == NULL) {
        printf("cd: type directory to move in\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("cd");
        }
    }
}

void shell_exit(char **args) {
    printf("exit shell\n");
    exit(0);
}

void shell_type(char **args) {
    if (args[1] == NULL) {
        printf("type command\n");
        return;
    }

    for (int i = 0; i < num_builtins(); i++) {
        if (strcmp(args[1], builtin_str[i]) == 0) {
            printf("%s: builtin command\n", args[1]);
            return;
        }
    }

    printf("%s: not builtin command\n", args[1]);
}

// 사용자 입력을 공백으로 나누는 함수
char **split_line(char *line) {
    int bufsize = 64, position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    token = strtok(line, " \t\r\n");
    while (token != NULL) {
        tokens[position++] = token;

        if (position >= bufsize) {
            bufsize += 64;
            tokens = realloc(tokens, bufsize * sizeof(char *));
        }

        token = strtok(NULL, " \t\r\n");
    }
    tokens[position] = NULL;
    return tokens;
}

// 명령 실행 함수
void execute(char **args) {
    if (args[0] == NULL) return;

    for (int i = 0; i < num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            (*builtin_func[i])(args);
            return;
        }
    }

    printf("%s: none\n", args[0]);
}

// 쉘 루프 함수
void shell_loop() {
    char *line = NULL;
    size_t len = 0;
    char **args;

    while (1) {
        printf("myshell$ ");
        getline(&line, &len, stdin);
        args = split_line(line);
        execute(args);
        free(args);
    }

    free(line);
}

// 메인 함수
int main() {
    shell_loop();
    return 0;
}
