/*
 * shell.c
 *
 * A simple shell program that might evolve.
 */

#include "shell.h"

int main()
{
    repl();
}

/*
 * Read, evaluate, and print. This is the basic functionality.
 */
void repl()
{
    // Declare variables.
    char line[SHELL_MAX_LINE_LENGTH];
    char *readerr;

    // Loop forever!
    while(1) {
        // Zero out the line. Slow, but...worth it maybe?
        for(int i = 0; i < SHELL_MAX_LINE_LENGTH; i++) {
            line[i] = '\0';
        }

        // Print the prompt.
        show_prompt();

        // Read a line.
        readerr = fgets(line, SHELL_MAX_LINE_LENGTH, stdin);
        if(NULL == readerr) {
            // Assume EOF.
            // TODO how do I tell if this is an error?
            close_shell(1, 0);
        }

        // Print the line.
        fprintf(stdout, "%s", line);
    }
}

/*
 * Print the prompt. Could be more advanced perhaps?
 */
void show_prompt(void)
{
    fprintf(stdout, "\033[38;5;47mrsh \033[38;5;12m$\033[00m ");
}

/*
 * Close the shell.
 */
void close_shell(int print_exit, int exit_status)
{
    if(print_exit) {
        fprintf(stdout, "exit\n");
    }
    exit(exit_status);
}
