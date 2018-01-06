/*
 * tokenizer.c
 *
 * Holds a tokenizer for rsh script. The main function of note
 * is next_token(), which returns a pointer to the next token
 * in the input.
 */

#include "tokenizer.h"

/*
 * Eat whitespace (space, tab, newline) from stdin until a non-whitespace
 * character is encountered. Return that character.
 */
void consume_whitespace()
{
    // Declare the character we'll read into.
    int c;

    // Get c using getchar(), then compare c to whitespace. This repeats
    // until c is not whitespace.
    while(isspace(c=getc(stdin)));

    // We fail on a read error.
    if(ferror(stdin)) {
        fprintf(stderr, "The tokenizer could not read from stdin. Exiting.\n");
        exit(1);
    }

    // Put c back (doesn't do anything if c is EOF).
    ungetc(c, stdin);

    // Return.
    return;
}

/*
 * Consume a word, turn it into a token, and return (a pointer to) that token.
 *
 * Words start with a-z, A-Z, or _, and consist of a-z, A-Z, 0-9, and _.
 */
t_token *consume_word()
{
    // Malloc a new token.
    t_token *ret = (t_token *)malloc(sizeof(t_token));
    if(!ret) {
        perror("Tokenizer could not malloc for a new token: ");
        exit(1);
    }

    // Set the type of the token.
    ret->type = word;

    // Set the value of the token.
    ret->value = consume_token(starts_word, word_contains);
    // If we got NULL from consume_token, we have an error. Free and exit.
    if(!(ret->value)) {
        free(ret);
        exit(1);
    }

    // Return.
    return ret;
}

/*
 * Return 1 if c starts a word, 0 otherwise.
 */
int starts_word(int c)
{
    if(isalpha(c)) {
        return 1;
    }
    if('_' == c) {
        return 1;
    }
    return 0;
}

/*
 * Return 1 if c can be in a word, 0 otherwise.
 */
int word_contains(int c)
{
    if(isalpha(c)) {
        return 1;
    }
    if(isdigit(c)) {
        return 1;
    }
    if('_' == c) {
        return 1;
    }
    return 0;
}

/*
 * Consume a token into a null-terminated string.
 */
char *consume_token(int (*starts)(int), int (*contains)(int))
{
    // We'll get characters into c.
    int c;

    // We'll put the characters we read into s.
    char *s = (char *)malloc(RSH_DEFAULT_TOKEN_LENGTH * sizeof(char));
    if(!s) {
        perror("Tokenizer could not malloc for a new token value: ");
        return NULL;
    }
    // Null-terminate to start off, just to be safe.
    s[0] = '\0';

    // Get the first character of the token. If it's not valid, unget and return.
    c = getc(stdin);
    if(!(*starts)(c)) {
        ungetc(c, stdin);
        return s;
    }
    // Set the first character and null-terminate.
    s[0] = c;
    s[1] = '\0';

    // Get the rest of the token. Current max length is 7.
    for(int i = 1; i < RSH_DEFAULT_TOKEN_LENGTH-1; i++) {
        // Get a character. If it's not valid, unget and return.
        c = getc(stdin);
        if(!(*contains)(c)) {
            ungetc(c, stdin);
            return s;
        }
        // Add the character to the token and null-terminate.
        s[i] = c;
        s[i+1] = '\0';
    }

    // Return the string.
    return s;
}

/*
 * A main function for testing.
 */
int main() {
    t_token *token = consume_word();
    printf("got token: \"%s\"\n", token->value);
    free(token->value);
    free(token);
}
