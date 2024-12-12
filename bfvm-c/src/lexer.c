/**
 * @file   lexer.h
 * @brief  Implementation of the lexical analyzer for BFVM's internal
 *         compiler.
 * @author Ruan C. Keet
 * @date   03-11-2023
 */

#include "lexer.h"
#include "error.h"

/* --- helper macros --------------------------------------------------------*/

#define BF_CMD(ch) (ch == '+' || ch == '-' || ch == '>' ||\
                    ch == '<' || ch == '.' || ch == ',' ||\
                    ch == ']' || ch == '[')

/* --- global variables -----------------------------------------------------*/

static FILE *source;
static char ch;

/* --- function prototypes --------------------------------------------------*/

/**
 * Retrieves the next character from the input stream.
 */
static void nextch(void);

/* --- lexer interface ------------------------------------------------------*/

void init_lexer(FILE *src) {
    source = src;
    position.line = 1;
    position.column = 0;
}

void next_token(token_t *const token) {
    do {
        nextch();
    } while (!BF_CMD(ch) && ch != EOF);

    *token = ch;
}

/* --- utility functions ----------------------------------------------------*/

static void nextch(void) {
    static char last = 0x00;

    ch = (char)fgetc(source);
    if (last == 0x0A && ch != EOF) {
        position.line++;
        position.column = 1;
    } else if (ch == EOF) {
        return;
    } else {
        position.column++;
    }

    last = ch;
}
