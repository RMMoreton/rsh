#ifndef RMORETON_SIMPLE_SHELL
#define RMORETON_SIMPLE_SHELL

#include <stdio.h>
#include <stdlib.h>

#define SHELL_MAX_LINE_LENGTH 160

void repl(void);
void show_prompt(void);
void close_shell(int, int);

#endif
