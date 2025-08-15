#ifndef LEXER_H
#define LEXER_H

#include "core/error.h"

typedef struct BfcLexer BfcLexer;

typedef enum BfcToken
{
    TOK_EOF         = -1,   /* EOF */
    TOK_ADD         = 0x2B, /* + */
    TOK_SUB         = 0x2D, /* - */
    TOK_ARROW_RIGHT = 0x3E, /* > */
    TOK_ARROW_LEFT  = 0x3C, /* < */
    TOK_DOT         = 0x2E, /* . */
    TOK_COMMA       = 0x2C, /* , */
    TOK_BRACE_LEFT  = 0x5B, /* [ */
    TOK_BRACE_RIGHT = 0x5D  /* ] */
} BfcToken;

BfcLexer *bfcInitLexer(const char *filepath);
void bfcCloseLexer(BfcLexer *lexer);

void bfcNextToken(BfcLexer *lexer, BfcToken *token);

BfcErrorSink bfcGetErrorSink(const BfcLexer *lexer);
BfcSourcePosition bfcGetCurrentSourcePosition(const BfcLexer *lexer);

#endif /* LEXER_H */
