#ifndef UTILS_H
#define UTILS_H

int isNumber(const char* str);
void testParsingCommand(char* input);
int isBuiltin(const char *keyword);
int isExternalCommand(const char *command);
int isOnlyWhitespace(const char *str);

#endif