#ifndef RMORETON_SIMPLE_SHELL_STRING
#define RMORETON_SIMPLE_SHELL_STRING

#include <stdio.h>
#include <string.h>

int tokenize(char *, char **, int);
char *shell_strtok_r(char *, char, char **);
char *strip_whitespace(char *);

#endif
