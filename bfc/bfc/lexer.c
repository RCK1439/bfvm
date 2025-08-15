#include "lexer.h"

#include "core/error.h"
#include "core/memory.h"

#include <stdio.h>
#include <stdlib.h>

#define BF_CMD(ch) (ch == '+' || ch == '-' || ch == '>' ||\
                    ch == '<' || ch == '.' || ch == ',' ||\
                    ch == ']' || ch == '[')

struct BFLexer
{
    FILE       *source;
    BFErrorSink sink;
    char        currentCharacter;
};

static void bfcNextCharacter(BFLexer *lexer);

BFLexer *bfcInitLexer(const char *filepath)
{
    const BFErrorSink sink = bfcInitErrorSink(filepath);
    
    FILE *source = NULL;
    if (filepath)
    {
#if defined(BFC_PLATFORM_LINUX)
        if ((source = fopen(filepath, "r")) == NULL)
        {
            bfcPrintError(sink, "could not open file: %s", filepath);
            bfcCloseErrorSink(sink);
            return NULL;
        }
#elif defined (BFC_PLATFORM_WINDOWS)
        if (fopen_s(&source, filepath, "r") != 0)
        {
            bfcPrintError(sink, "could not open file: %s", filepath);
            bfcCloseErrorSink(sink);
            return NULL;
        }
#endif
    }
    else
    {
        bfcPrintInfo(sink, "enter commands:");
        source = stdin;
    }

    BFLexer *const lexer = BFC_MALLOC(BFLexer, 1);
    lexer->source = source;
    lexer->currentCharacter = 0x00;
    lexer->sink = sink;
    
    return lexer;
}

void bfcCloseLexer(BFLexer *lexer)
{
    if (lexer->source != stdin)
    {
        fclose(lexer->source);
    }

    bfcCloseErrorSink(lexer->sink);
    BFC_FREE(lexer);
}

void bfcNextToken(BFLexer *lexer, BFToken *token)
{
    do
    {
        bfcNextCharacter(lexer);
    } while (!BF_CMD(lexer->currentCharacter) && lexer->currentCharacter != EOF);

    *token = (BFToken)lexer->currentCharacter;
}

BFErrorSink bfcGetErrorSink(const BFLexer *lexer)
{
    return lexer->sink;
}

BFSourcePosition bfcGetCurrentSourcePosition(const BFLexer *lexer)
{
    return lexer->sink.position;
}

static void bfcNextCharacter(BFLexer *lexer)
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
