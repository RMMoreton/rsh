/*
 * shell_string.c
 *
 * Holds string functions for my shell program.
 */

#include "shell_string.h"

/*
 * Checks if a string contains a character.
 *
 * The string must be null termianted!
 */
int str_contains(char *str, char c)
{
    // Loop over the string until the null terminator, checking for c.
    while('\0' != *str) {
        if(c == *(str++)) {
            return 1;
        }
    }
    return 0;
}

/*
 * Get a line of arbitrary length, resizing the array as necessary.
 *
 * The calling function must free the returned pointer!
 */
char *get_line(void)
{
    // Declare variables.
    char *line;
    char *readerr;
    int len = DEFAULT_SHELL_LINE_LENGTH;
    int used_len = 0;

    // Do an initial malloc for the line.
    line = (char *)malloc(len * sizeof(char));
    if(NULL == line) {
        fprintf(stderr, "could not malloc for input line");
        return NULL;
    }

    // Loop until we've finished reading the line.
    while(1) {
        // Read in some stuff.
        readerr = fgets(line+used_len, len-used_len, stdin);
        if(NULL == readerr) {
            // We read in EOF, or we got an error. Return null I guess?
            free(line);
            return NULL;
        }

        // Check if it contains a newline. If so, we're done!
        if(str_contains(line, '\n')) {
            return line;
        }

        // Get a new array that's bigger than the last one.
        used_len = len - 1;
        reallocate_str(&line, &len);
        if(line == NULL) {
            fprintf(stderr, "could not malloc enough space for input line\n");
            return NULL;
        }
    }
}

/*
 * Malloc a new array of size 2*len, copy line into it, and free line.
 */
void reallocate_str(char **line_ptr, int *len)
{
    // Declare variables.
    char *new_line;
    int old_len = *len;

    // Double the line length here and in the calling function.
    *len = *len * 2;

    // Malloc the new array.
    new_line = (char *)malloc(*len * sizeof(char));
    if(NULL == new_line) {
        free(*line_ptr); // A partial line is useless - throw it all out.
        *line_ptr = NULL;
        return;
    }

    // Copy line into new_line.
    for(int i = 0; i < old_len; i++) {
        new_line[i] = (*line_ptr)[i];
    }

    // Clean up and return.
    free(*line_ptr);
    *line_ptr = new_line;
    return;
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

/*
 * strip_whitespace strips leading and trailing whitespace from a character
 * array.
 *
 * This changes the first argument!
 * The array must be null terminated!
 */
char *strip_whitespace(char *line)
{
    // Declare variables.
    char *end;

    // Move line up until it points to non-whitespace (i.e. space, tab,
    // newline).
    while(*line == ' ' || *line == '\t' || *line == '\n') {
        ++line;
    }

    // Move end to the end of the string.
    end = line;
    while(*end != '\0') {
        ++end;
    }
    
    // Move end back to the first non-whitespace character.
    while(end > line && (*end == ' '
                || *end == '\t'
                || *end == '\n'
                || *end == '\0')) {
        *(end--) = '\0';
    }

    // Return start. It points to a null-terminated string with no leading
    // or trailing whitespace.
    return line;
}
