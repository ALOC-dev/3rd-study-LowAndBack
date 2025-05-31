#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "builtin.h"

// 내장 명령어 함수 선언
void shell_echo(char **args);
void shell_pwd(char **args);
void shell_cd(char **args);
void shell_exit(char **args);
void shell_type(char **args);
void shell_export(char **args);

// 내장 명령어 이름 배열
char *builtin_str[] = {
    "echo",
    "pwd",
    "cd",
    "exit",
    "type",
    "export"
};

// 내장 명령어 함수 포인터 배열
void (*builtin_func[])(char **) = {
    &shell_echo,
    &shell_pwd,
    &shell_cd,
    &shell_exit,
    &shell_type,
    &shell_export
};

int num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

// echo 명령어: 인자들 출력 (write 사용)
void shell_echo(char **args) {
    for (int i = 1; args[i] != NULL; i++) {
        write(STDOUT_FILENO, args[i], strlen(args[i]));
        if (args[i + 1] != NULL)
            write(STDOUT_FILENO, " ", 1);
    }
    write(STDOUT_FILENO, "\n", 1);
}

// pwd 명령어: 현재 디렉터리 출력 (write 사용)
void shell_pwd(char **args) {
    (void)args;  // 사용 안함 표시
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        write(STDOUT_FILENO, cwd, strlen(cwd));
        write(STDOUT_FILENO, "\n", 1);
    } else {
        const char *msg = "pwd: error getting current directory\n";
        write(STDERR_FILENO, msg, strlen(msg));
    }
}

// cd 명령어: 디렉터리 변경
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

// exit 명령어: 쉘 종료
void shell_exit(char **args) {
    (void)args;
    const char *msg = "exit\n";
    write(STDOUT_FILENO, msg, strlen(msg));
    exit(0);
}

// type 명령어: 내장 명령어 여부 출력 (write 사용)
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

// export 명령어: 환경변수 설정 및 출력 (write 사용)
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

            // 변수명 첫 글자 검사 (영문자 또는 _)
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
            // 등호 없는 경우 빈 문자열 값 설정
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

int execute_builtin(char *const args[]) {
    if (args[0] == NULL)
        return -1;

    for (int i = 0; i < num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            (*builtin_func[i])((char **)args);
            return 0;
        }
    }
    return -1;
}
