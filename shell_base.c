/*
 * shell.c
 *
 * A simple shell program that might evolve.
 */

#include "shell_base.h"

/*
 * The main entrypoint of the shell. Right now we just call the REPL method.
 */
int main()
{
    repl();
}

/*
 * Read a line, evaluate it, loop.
 */
void repl()
{
    // Declare variables.
    char *line = NULL;

    // Loop forever!
    while(1) {
        // If line is not null, free it.
        if(NULL != line) {
            free(line);
        }

        // Print the prompt.
        show_prompt();

        // Read a line.
        line = get_line();
        if(NULL == line) {
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
void evaluate(char *line)
{
    // Declare variables.
    char *tokens[DEFAULT_SHELL_LINE_LENGTH];
    char *command;
    int numtokens;

    // All I care about is the stuff before the first newline character.
    // TODO care about stuff after the first newline character?
    line = strip_whitespace(line);

    // Tokenize the line.
    numtokens = tokenize(line, tokens, DEFAULT_SHELL_LINE_LENGTH - 1);

    // Set tokens[numtokens+1] to NULL to terminate the sequence.
    tokens[numtokens+1] = NULL;

    // Switch on the first token.
    command = tokens[0];
    if(NULL == command) {
        // Do nothing.
    } else if(0 == strcmp(command, "exit")) {
        // Close the shell. Don't print "exit".
        free(line);
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
