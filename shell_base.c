/*
 * shell.c
 *
 * A simple shell program that might evolve.
 */

#include "shell_base.h"

/*
 * Read a line, evaluate it, loop.
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
            // TODO how do I tell if this is an error vs EOF?
            close_shell(1, 0);
        }

        // Evaluate the line.
        evaluate(line);
    }
}

/*
 * Print the prompt.
 */
void show_prompt(void)
{
    fprintf(stdout, "\033[38;5;47mrsh \033[38;5;12m$\033[00m ");
}

/*
 * Evaluate a line.
 */
void evaluate(char line[SHELL_MAX_LINE_LENGTH])
{
    // Declare variables.
    char *tokens[SHELL_MAX_LINE_LENGTH];
    char *command;
    int numtokens;

    // Strip the newline from the end.
    strip_newline(line);

    // Tokenize the line.
    numtokens = tokenize(line, tokens, SHELL_MAX_LINE_LENGTH - 1);

    // Set tokens[numtokens+1] to NULL to terminate the sequence.
    tokens[numtokens+1] = NULL;

    // Switch on the first token.
    command = tokens[0];
    if(NULL == command) {
        // Do nothing.
    } else if(0 == strcmp(command, "exit")) {
        // Close the shell. Don't print "exit".
        close_shell(0, 0);
    } else {
        // Attempt to execute the command.
        execute_command(tokens);
    }
}

/*
 * Execute a command in the shell.
 */
void execute_command(char **tokens)
{
    // Declare variables.
    pid_t child_pid;
    int status;

    // Fork.
    child_pid = fork();

    // Error check.
    if(child_pid < 0) {
        perror("execute_command");
        return;
    }

    if(child_pid) {
        // If we're the parent, wait on the child.
        waitpid(child_pid, &status, 0);
        return;
    } else {
        // If we're the child, execute the command.
        status = execvp(tokens[0], tokens);
        // Error! Print the error and exit this shell.
        perror(tokens[0]);
        close_shell(0, status);
    }
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
