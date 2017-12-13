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
 * Print the character values of everything up the the null char.
 */
void printchars(char *line)
{
    for(int i = 0; ; i++) {
        if(line[i] == '\0') {
            printf("NULLCHAR\n");
            return;
        }
        printf("%d ", (int)line[i]);
    }
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

    // Strip the newline from the line.
    strip_newline(line);

    // Tokenize the line.
    tokenize(line, tokens, SHELL_MAX_LINE_LENGTH - 1);

    // Switch on the first token (allows for shell commands).
    command = tokens[0];
    if(NULL == command) {
        // Do nothing.
    } else if(!strcmp(command, "exit")) {
        // Close the shell. Don't print "exit".
        close_shell(0, 0);
    } else {
        execute_command(tokens);
    }
}

/*
 * Replace the first occurence of a newline character with a null character.
 *
 * Line must be null terminated!
 */
void strip_newline(char *line)
{
    while(1) {
        if(*line == '\0' || *line == '\n') {
            *line = '\0';
            return;
        }
        line++;
    }
}

/*
 * Tokenize a line by spaces (ignore quotes).
 *
 * max_tokens must be > 0
 */
int tokenize(char *line, char **tokens, int max_tokens)
{
    // Declare variables.
    int i;
    char *saveptr;

    // Error check.
    if(max_tokens <= 0) {
        fprintf(stderr, "attempted to tokenize input, but max_tokens was 0");
        return 0;
    }

    // Loop over the line and get tokens from it.
    for(i=0; ; i++, line=NULL) {
        // Don't tokenize too much! If the string can't be fully tokenized,
        // we just drop the end. That's not great.
        // TODO don't just drop the end of the string.
        if(i >= max_tokens-1) {
            break;
        }
        // Get a new token. Break if it's NULL.
        tokens[i] = strtok_r(line, ' ', &saveptr);
        if(tokens[i] == NULL) {
            break;
        }
    }

    // Return how many tokens we read.
    return i;
}

/*
 * Get a token from a string.
 *
 * This changes first argument!
 * The line must be null-terminated!
 */
char *strtok_r(char *line, char delim, char **saveptr)
{
    // Declare variables.
    char *start;
    char *end;

    // Error check.
    if(delim == '\0') {
        fprintf(stderr, "cannot tokenize on the null character");
        return NULL;
    }

    // Set start appropriately.
    if(NULL != line) {
        start = line;
    } else if(NULL != saveptr) {
        start = *saveptr;
    } else {
        fprintf(stderr, "must provide either line or saveptr");
        return NULL;
    }

    // Move start up to the first non-delim character.
    while(1) {
        if(*start != delim) {
            break;
        }
        start++;
    }

    // If start is on the null character, return null.
    if(*start == '\0') {
        *saveptr = start;
        return NULL;
    }

    // Move end until the first delim character.
    end = start + 1;
    while(1) {
        if(*end == delim || *end == '\0') {
            break;
        }
        end++;
    }

    // Replace delim, set saveptr.
    if(*end == '\0') {
        *saveptr = end;
    } else {
        *end = '\0';
        *saveptr = end + 1;
    }

    // Return the start of the token.
    return start;
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
