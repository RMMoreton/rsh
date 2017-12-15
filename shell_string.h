#ifndef RMORETON_SIMPLE_SHELL_STRING
#define RMORETON_SIMPLE_SHELL_STRING

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEFAULT_SHELL_LINE_LENGTH 8

int str_contains(char *, char);
char *get_line(void);
void reallocate_str(char **, int *);
int tokenize(char *, char **, int);
char *shell_strtok_r(char *, char, char **);
char *strip_whitespace(char *);

#endif
