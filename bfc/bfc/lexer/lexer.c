#include "lexer.h"

#include "core/error.h"
#include "core/memory.h"

#include <stdio.h>
#include <string.h>

#define IS_BF_CMD(ch)                            \
    ((ch) == '+' || (ch) == '-' || (ch) == '>' ||\
     (ch) == '<' || (ch) == '.' || (ch) == ',' ||\
     (ch) == ']' || (ch) == '[')                 \

struct BFLexer
{
    BFSourcePosition position;
    char            *programName;
    FILE            *source;
    char             currentCharacter;
};

static void bfcNextCharacter(BFLexer *lexer);

BFLexer *bfcInitLexer(const char *filepath)
{
#if defined(BFC_PLATFORM_LINUX)
    FILE *const source = fopen(filepath, "r");
    if (!source)
    {
        bfcPrintError("could not open file: %s", filepath);
        return NULL;
    }
#elif defined (BFC_PLATFORM_WINDOWS)
    FILE *const source;
    if (fopen_s(&source, filepath, "r") != 0)
    {
        bfcPrintError("could not open file: %s", filepath);
        return NULL;
    }
#endif

    const char *c = strchr(filepath, '/');
    if (!c)
    {
        c = (char *)filepath;
    }
    else
    {
        c++;
    }

    BFLexer *const lexer = BFC_MALLOC(BFLexer, 1);
    lexer->position.line = 1;
    lexer->position.column = 0;
    lexer->programName = bfcCloneString(c);
    lexer->source = source;
    lexer->currentCharacter = 0x00;

    return lexer;
}

void bfcCloseLexer(BFLexer *lexer)
{
    fclose(lexer->source);
    BFC_FREE(lexer->programName);
    BFC_FREE(lexer);
}

BFToken bfcNextToken(BFLexer *lexer)
{
    do
    {
        bfcNextCharacter(lexer);
    } while (!IS_BF_CMD(lexer->currentCharacter) && lexer->currentCharacter != EOF);

    return (BFToken)lexer->currentCharacter;
}

BFSourcePosition bfcGetCurrentSourcePosition(const BFLexer *lexer)
{
    return lexer->position;
}

const char *bfcGetProgramName(const BFLexer *lexer)
{
    return lexer->programName;
}

static void bfcNextCharacter(BFLexer *lexer)
{
    static char last = 0x00;

    lexer->currentCharacter = (char)fgetc(lexer->source);
    if (last == 0x0A && lexer->currentCharacter != EOF)
    {
        lexer->position.line++;
        lexer->position.column = 1;
    }
    else if (lexer->currentCharacter == EOF)
    {
        return;
    }
    else
    {
        lexer->position.column++;
    }

    last = lexer->currentCharacter;
}
