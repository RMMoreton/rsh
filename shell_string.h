#ifndef RMORETON_SIMPLE_SHELL_STRING
#define RMORETON_SIMPLE_SHELL_STRING

#include <stdio.h>
#include <string.h>

void strip_newline(char *);
int tokenize(char *, char **, int);
char *strtok_r(char *, char, char **);

#endif
