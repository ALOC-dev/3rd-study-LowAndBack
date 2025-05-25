#ifndef PARSER_H
#define PARSER_H

typedef struct ArgNode {
    char *arg;
    struct ArgNode *next;
} ArgNode;

typedef struct ParsedCommand {
    char *keyword;      // 명령어 (첫 단어)
    ArgNode *args;      // 인자 리스트 (연결 리스트)
    int argc;           // 인자 개수
} ParsedCommand;

/**
 * 명령어 인자 리스트에 새 인자를 추가
 * @param cmd 파싱된 명령어 구조체 포인터
 * @param arg_str 새 인자 문자열
 */
void add_arg(ParsedCommand *cmd, const char *arg_str);

/**
 * 입력 문자열을 파싱하여 ParsedCommand 구조체로 반환
 * @param input 입력 문자열
 * @return 동적 할당된 ParsedCommand 포인터 (사용 후 해제 필요)
 */
ParsedCommand *parse_input(const char *input);

/**
 * 파싱된 명령어 내용을 출력
 * @param cmd 출력할 ParsedCommand 포인터
 */
void print_command(ParsedCommand *cmd);

#endif // PARSER_H
