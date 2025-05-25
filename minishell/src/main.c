#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "builtin.h"

int main(void) {
    char line[1024];

    while (1) {
        fputs("$ >  ", stdout);
        if (!fgets(line, sizeof(line), stdin)) {
            break;
        }

        line[strcspn(line, "\n")] = '\0';

        if (line[0] == '\0') continue;

        // 파싱
        ParsedCommand *cmd = parse_input(line);
        //print_parsed_command(cmd);
        run_builtin(cmd);
        
        
        
        // 메모리 해제
        free_parsed_command(cmd);
    }
}