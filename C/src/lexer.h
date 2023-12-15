/**
 * @file   lexer.h
 * @brief  The lexical analyzer for BFVM's internal compiler.
 * @author Ruan C. Keet
 * @date   03-11-2023
 */

#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

/* --- type definitions -----------------------------------------------------*/

/**
 * Describes the tokens that can be read from the src
 * stream.
 */
typedef enum {
    TOK_EOF         = -1,   /* EOF */
    TOK_ADD         = 0x2B, /* + */
    TOK_SUB         = 0x2D, /* - */
    TOK_ARROW_RIGHT = 0x3E, /* > */
    TOK_ARROW_LEFT  = 0x3C, /* < */
    TOK_DOT         = 0x2E, /* . */
    TOK_COMMA       = 0x2C, /* , */
    TOK_BRACE_LEFT  = 0x5B, /* [ */
    TOK_BRACE_RIGHT = 0x5D  /* ] */
} Token;

/* --- lexer interface ------------------------------------------------------*/

/**
 * Initializes the lexer.
 * 
 * @param[in] src
 *      The stream to read the source code from.
 */
void init_lexer(FILE *src);

/**
 * Gets the next token from the stream.
 * 
 * @param[out] token
 *      The token read from the stream.
 */
void next_token(Token *token);

#endif /* LEXER_H */
