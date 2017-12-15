#ifndef RMORETON_SIMPLE_SHELL_BASE
#define RMORETON_SIMPLE_SHELL_BASE

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "shell_string.h"

void repl(void);
void show_prompt(void);
void evaluate(char *);
void execute_command(char **);
void close_shell(int, int);

#endif
