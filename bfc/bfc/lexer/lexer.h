#ifndef LEXER_H
#define LEXER_H

#include "core/types.h"

typedef struct BFLexer BFLexer;

typedef struct BFSourcePosition
{
    size_t line;
    size_t column;
} BFSourcePosition;

typedef enum BFToken
{
    TOK_EOF         = -1,   /* EOF */
    TOK_ADD         = 0x2B, /*  +  */
    TOK_SUB         = 0x2D, /*  -  */
    TOK_ARROW_RIGHT = 0x3E, /*  >  */
    TOK_ARROW_LEFT  = 0x3C, /*  <  */
    TOK_DOT         = 0x2E, /*  .  */
    TOK_COMMA       = 0x2C, /*  ,  */
    TOK_BRACE_LEFT  = 0x5B, /*  [  */
    TOK_BRACE_RIGHT = 0x5D  /*  ]  */
} BFToken;

BFLexer *bfcInitLexer(const char *filepath);
void bfcCloseLexer(BFLexer *lexer);

BFToken bfcNextToken(BFLexer *lexer);

BFSourcePosition bfcGetCurrentSourcePosition(const BFLexer *lexer);
const char *bfcGetProgramName(const BFLexer *lexer);

#endif /* LEXER_H */
