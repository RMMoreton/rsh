/*
 * shell_string.c
 *
 * Holds string functions for my shell program.
 */

#include "shell_string.h"

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
        if(i >= max_tokens) {
            break;
        }
        // Get a new token. Break if it's NULL.
        tokens[i] = shell_strtok_r(line, ' ', &saveptr);
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
 * This changes the first argument!
 * The line must be null-terminated!
 */
char *shell_strtok_r(char *line, char delim, char **saveptr)
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
