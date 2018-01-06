#ifndef RMORETON_RSH_TOKENIZER
#define RMORETON_RSH_TOKENIZER

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define RSH_DEFAULT_TOKEN_LENGTH 8

/*
 * Tokens must be one of the following types.
 */
enum rsh_token_type {
    word
};

/*
 * Tokens contain a type and a pointer to a null-terminated string
 * that contains their literal value.
 */
typedef struct token {
    enum rsh_token_type type;
    char *value;
} t_token;

void consume_whitespace(void);
t_token *consume_word(void);
int starts_word(int);
int word_contains(int);
char *consume_token(int (*fp0)(int), int (*fp1)(int));

#endif
