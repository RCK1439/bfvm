#include "lexer.h"

#include "core/error.h"
#include "core/memory.h"

#include <stdio.h>
#include <stdlib.h>

#define BF_CMD(ch) (ch == '+' || ch == '-' || ch == '>' ||\
                    ch == '<' || ch == '.' || ch == ',' ||\
                    ch == ']' || ch == '[')

struct BfcLexer
{
    FILE        *source;
    BfcErrorSink sink;
    char         currentCharacter;
};

static void bfcNextCharacter(BfcLexer *lexer);

BfcLexer *bfcInitLexer(const char *filepath)
{
    const BfcErrorSink sink = bfcInitErrorSink(filepath);
    
    FILE *source = NULL;
    if (filepath)
    {
#if defined(BFVM_LINUX)
        if ((source = fopen(filepath, "r")) == NULL)
        {
            bfcPrintError("could not open file: %s", filepath);
            return NULL;
        }
#elif defined (BFVM_WINDOWS)
        if (fopen_s(&source, filepath, "r") != 0)
        {
            bfcPrintError("could not open file: %s", filepath);
            return NULL;
        }
#endif
    }
    else
    {
        bfcPrintInfo(sink, "enter commands:");
        source = stdin;
    }

    BfcLexer *const lexer = BFC_MALLOC(BfcLexer, 1);
    lexer->source = source;
    lexer->currentCharacter = 0x00;
    lexer->sink = sink;
    
    return lexer;
}

void bfcCloseLexer(BfcLexer *lexer)
{
    if (lexer->source != stdin)
    {
        fclose(lexer->source);
    }

    bfcCloseErrorSink(lexer->sink);
    BFC_FREE(lexer);
}

void bfcNextToken(BfcLexer *lexer, BfcToken *token)
{
    do
    {
        bfcNextCharacter(lexer);
    } while (!BF_CMD(lexer->currentCharacter) && lexer->currentCharacter != EOF);

    *token = (BfcToken)lexer->currentCharacter;
}

BfcErrorSink bfcGetErrorSink(const BfcLexer *lexer)
{
    return lexer->sink;
}

BfcSourcePosition bfcGetCurrentSourcePosition(const BfcLexer *lexer)
{
    return lexer->sink.position;
}

static void bfcNextCharacter(BfcLexer *lexer)
{
    static char last = 0x00;

    lexer->currentCharacter = (char)fgetc(lexer->source);
    if (last == 0x0A && lexer->currentCharacter != EOF)
    {
        lexer->sink.position.line++;
        lexer->sink.position.column = 1;
    }
    else if (lexer->currentCharacter == EOF)
    {
        return;
    }
    else
    {
        lexer->sink.position.column++;
    }

    last = lexer->currentCharacter;
}
