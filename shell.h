#ifndef RMORETON_SIMPLE_SHELL
#define RMORETON_SIMPLE_SHELL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SHELL_MAX_LINE_LENGTH 160

void repl(void);
void show_prompt(void);
void evaluate(char *);
void strip_newline(char *);
int tokenize(char *, char **, int);
char *strtok_r(char *, char, char **);
void close_shell(int, int);

#endif
