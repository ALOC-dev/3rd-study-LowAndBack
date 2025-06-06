#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdbool.h>
#include <errno.h>

// 내장 명령어 함수 선언
void shell_echo(char **args);
void shell_pwd(char **args);
void shell_cd(char **args);
void shell_exit(char **args);
void shell_type(char **args);
void shell_export(char **args);

char *builtin_str[] = {
    "echo", "pwd", "cd", "exit", "type", "export"
};

void (*builtin_func[])(char **) = {
    shell_echo, shell_pwd, shell_cd, shell_exit, shell_type, shell_export
};

int num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

// 시그널 처리 함수들
void sigint_handler(int signo) {
    write(STDOUT_FILENO, "\nmini-shell> ", 13);
}

void sigtstp_handler(int signo) {
    write(STDOUT_FILENO, "\nmini-shell> ", 13);
}

// 내장 명령어 구현들
void shell_echo(char **args) {
    for (int i = 1; args[i] != NULL; i++) {
        write(STDOUT_FILENO, args[i], strlen(args[i]));
        if (args[i + 1] != NULL)
            write(STDOUT_FILENO, " ", 1);
    }
    write(STDOUT_FILENO, "\n", 1);
}

void shell_pwd(char **args) {
    (void)args;
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        write(STDOUT_FILENO, cwd, strlen(cwd));
        write(STDOUT_FILENO, "\n", 1);
    } else {
        const char *msg = "pwd: error getting current directory\n";
        write(STDERR_FILENO, msg, strlen(msg));
    }
}

void shell_cd(char **args) {
    if (args[1] == NULL) {
        const char *msg = "cd: expected argument\n";
        write(STDERR_FILENO, msg, strlen(msg));
    } else {
        if (chdir(args[1]) != 0) {
            perror("cd");
        }
    }
}

void shell_exit(char **args) {
    (void)args;
    const char *msg = "exit\n";
    write(STDOUT_FILENO, msg, strlen(msg));
    exit(0);
}

void shell_type(char **args) {
    if (args[1] == NULL) {
        const char *msg = "type: expected argument\n";
        write(STDERR_FILENO, msg, strlen(msg));
        return;
    }
    for (int i = 0; i < num_builtins(); i++) {
        if (strcmp(args[1], builtin_str[i]) == 0) {
            write(STDOUT_FILENO, args[1], strlen(args[1]));
            write(STDOUT_FILENO, ": builtin command\n", 18);
            return;
        }
    }
    write(STDOUT_FILENO, args[1], strlen(args[1]));
    write(STDOUT_FILENO, ": not builtin command\n", 22);
}

void shell_export(char **args) {
    if (args[1] == NULL) {
        extern char **environ;
        for (int i = 0; environ[i] != NULL; i++) {
            const char *prefix = "declare -x ";
            write(STDOUT_FILENO, prefix, strlen(prefix));
            write(STDOUT_FILENO, environ[i], strlen(environ[i]));
            write(STDOUT_FILENO, "\n", 1);
        }
        return;
    }

    for (int i = 1; args[i] != NULL; i++) {
        char *arg = args[i];
        char *eq = strchr(arg, '=');
        if (eq != NULL) {
            size_t var_len = eq - arg;
            if (var_len == 0) {
                const char *msg = "export: invalid variable name\n";
                write(STDERR_FILENO, msg, strlen(msg));
                continue;
            }
            if (!((arg[0] >= 'A' && arg[0] <= 'Z') ||
                  (arg[0] >= 'a' && arg[0] <= 'z') ||
                  (arg[0] == '_'))) {
                char buf[300];
                int n = snprintf(buf, sizeof(buf), "export: invalid variable name: %.*s\n", (int)var_len, arg);
                write(STDERR_FILENO, buf, n);
                continue;
            }
            char var_name[256];
            if (var_len >= sizeof(var_name)) var_len = sizeof(var_name) - 1;
            strncpy(var_name, arg, var_len);
            var_name[var_len] = '\0';

            char *value = eq + 1;
            if (setenv(var_name, value, 1) != 0) {
                perror("export");
            }
        } else {
            if (setenv(arg, "", 1) != 0) {
                perror("export");
            }
        }
    }
}

bool is_builtin_command(const char *cmd) {
    for (int i = 0; i < num_builtins(); i++) {
        if (strcmp(cmd, builtin_str[i]) == 0)
            return true;
    }
    return false;
}

int execute_builtin(char **args) {
    if (args[0] == NULL)
        return -1;

    for (int i = 0; i < num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            builtin_func[i](args);
            return 0;
        }
    }
    return -1;
}

// 공백,& -> 백그라운드 실행
char **parse_line(char *line, bool *background) {
    size_t bufsize = 64;
    size_t position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    if (!tokens) {
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE);
    }

    *background = false;

    char *line_cpy = strdup(line);
    char *saveptr;
    token = strtok_r(line_cpy, " \t\r\n", &saveptr);
    while (token != NULL) {
        if (strcmp(token, "&") == 0) {
            *background = true;
        } else {
            tokens[position++] = strdup(token);
            if (position >= bufsize) {
                bufsize += 64;
                tokens = realloc(tokens, bufsize * sizeof(char *));
                if (!tokens) {
                    fprintf(stderr, "allocation error\n");
                    exit(EXIT_FAILURE);
                }
            }
        }
        token = strtok_r(NULL, " \t\r\n", &saveptr);
    }
    tokens[position] = NULL;

    free(line_cpy);
    return tokens;
}

void free_args(char **args) {
    for (int i = 0; args[i] != NULL; i++)
        free(args[i]);
    free(args);
}

void launch_process(char **args, bool background) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    } else if (pid == 0) {
        // 자식 프로세스

        // 시그널 기본 동작 복원
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);

        if (is_builtin_command(args[0])) {
            execute_builtin(args);
            exit(0);
        } else {
            // 외부 명령어 실행
            if (execvp(args[0], args) == -1) {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        }
    } else {
        // 부모 프로세스
        if (!background) {
            int status;
            waitpid(pid, &status, 0);
        } else {
            printf("[background pid %d]\n", pid);
        }
    }
}
