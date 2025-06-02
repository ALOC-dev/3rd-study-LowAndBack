#ifndef UTILS_H
#define UTILS_H

char *ft_strdup(const char *s);
void fatal_error(const char *msg);
int is_whitespace_only(const char *str);
char *get_env_value(const char *name);

#endif
