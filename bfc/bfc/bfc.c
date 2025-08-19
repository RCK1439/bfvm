#include "bfc.h"

#include "core/error.h"
#include "core/memory.h"

#include "lexer/lexer.h"

#define INIT_CODE_SIZE 32UL

#define PARSE_CHAIN(TOKEN, INSTR, OFFSET)                   \
    bfcEnsureCodeSpace(compiler);                           \
    compiler->code[compiler->pos].instr = INSTR;            \
    OFFSET = 0;                                             \
    while (compiler->currToken == TOKEN)                    \
    {                                                       \
        OFFSET++;                                           \
        compiler->currToken = bfcNextToken(compiler->lexer);\
    }                                                       \
    compiler->pos++                                         \

typedef struct BFCompiler
{
    BFLexer  *lexer;
    BFOpCode *code;
    size_t    pos;
    size_t    size;
    BFToken   currToken;
} BFCompiler;

static void bfcParseProgram(BFCompiler *compiler);
static void bfcParseAddByte(BFCompiler *compiler);
static void bfcParseSubByte(BFCompiler *compiler);
static void bfcParseAddPtr(BFCompiler *compiler);
static void bfcParseSubPtr(BFCompiler *compiler);
static void bfcParseWrite(BFCompiler *compiler);
static void bfcParseRead(BFCompiler *compiler);
static void bfcParseConditional(BFCompiler *compiler);

static void bfcEnsureCodeSpace(BFCompiler *compiler);
static void bfcDefer(BFCompiler *compiler);

const BFOpCode *bfcCompile(const char *filepath)
{
    BFLexer *const lexer = bfcInitLexer(filepath);
    if (!lexer)
    {
        return NULL;
    }

    BFCompiler *const compiler = BFC_MALLOC(BFCompiler, 1);
    compiler->lexer = lexer;
    compiler->code = BFC_MALLOC(BFOpCode, INIT_CODE_SIZE);
    compiler->pos = 0;
    compiler->size = INIT_CODE_SIZE;

    bfcParseProgram(compiler);
    BFOpCode *const code = compiler->code;

    bfcCloseLexer(compiler->lexer);
    BFC_FREE(compiler);

    return code;
}

/* --- parser routines ------------------------------------------------------*/

static void bfcParseProgram(BFCompiler *compiler)
{
    compiler->currToken = bfcNextToken(compiler->lexer);
    while (compiler->currToken != TOK_EOF)
    {
        switch (compiler->currToken)
        {
            case TOK_ADD:
                bfcParseAddByte(compiler);
                break;
            case TOK_SUB:
                bfcParseSubByte(compiler);
                break;
            case TOK_ARROW_RIGHT:
                bfcParseAddPtr(compiler);
                break;
            case TOK_ARROW_LEFT:
                bfcParseSubPtr(compiler);
                break;
            case TOK_DOT:
                bfcParseWrite(compiler);
                break;
            case TOK_COMMA:
                bfcParseRead(compiler);
                break;
            case TOK_BRACE_LEFT:
                bfcParseConditional(compiler);
                break;
            default:
            {
                const char *const progName = bfcGetProgramName(compiler->lexer);
                const BFSourcePosition pos = bfcGetCurrentSourcePosition(compiler->lexer);
                bfcPrintErrorPos(progName, pos.line, pos.column, "unknown token: %c", (char)compiler->currToken);
                bfcDefer(compiler);
            } return;
        }
    }

    if (!compiler->code)
    {
        return;
    }

    bfcEnsureCodeSpace(compiler);
    compiler->code[compiler->pos].instr = BFC_END;
}

static void bfcParseAddByte(BFCompiler *compiler)
{
    if (!compiler->code)
    {
        return;
    }

    PARSE_CHAIN(TOK_ADD, BFC_ADDB, compiler->code[compiler->pos].operands.byteOffset);
}

static void bfcParseSubByte(BFCompiler *compiler)
{
    if (!compiler->code)
    {
        return;
    }

    PARSE_CHAIN(TOK_SUB, BFC_SUBB, compiler->code[compiler->pos].operands.byteOffset);
}

static void bfcParseAddPtr(BFCompiler *compiler)
{
    if (!compiler->code)
    {
        return;
    }

    PARSE_CHAIN(TOK_ARROW_RIGHT, BFC_ADDP, compiler->code[compiler->pos].operands.dataOffset);
}

static void bfcParseSubPtr(BFCompiler *compiler)
{
    if (!compiler->code)
    {
        return;
    }

    PARSE_CHAIN(TOK_ARROW_LEFT, BFC_SUBP, compiler->code[compiler->pos].operands.dataOffset);
}

static void bfcParseWrite(BFCompiler *compiler)
{
    if (!compiler->code)
    {
        return;
    }

    bfcEnsureCodeSpace(compiler);

    compiler->code[compiler->pos++].instr = BFC_WRITE;
    compiler->currToken = bfcNextToken(compiler->lexer);
}

static void bfcParseRead(BFCompiler *compiler)
{
    if (!compiler->code)
    {
        return;
    }

    bfcEnsureCodeSpace(compiler);

    compiler->code[compiler->pos++].instr = BFC_READ;
    compiler->currToken = bfcNextToken(compiler->lexer);
}

static void bfcParseConditional(BFCompiler *compiler)
{
    if (!compiler->code)
    {
        return;
    }

    const size_t openPos = compiler->pos;
    const BFSourcePosition openSrcPos = bfcGetCurrentSourcePosition(compiler->lexer);

    bfcEnsureCodeSpace(compiler);
    compiler->code[compiler->pos++].instr = BFC_JZ;

    compiler->currToken = bfcNextToken(compiler->lexer);
    while (compiler->currToken != TOK_BRACE_RIGHT)
    {
        switch (compiler->currToken)
        {
            case TOK_ADD:
                bfcParseAddByte(compiler);
                break;
            case TOK_SUB:
                bfcParseSubByte(compiler);
                break;
            case TOK_ARROW_RIGHT:
                bfcParseAddPtr(compiler);
                break;
            case TOK_ARROW_LEFT:
                bfcParseSubPtr(compiler);
                break;
            case TOK_DOT:
                bfcParseWrite(compiler);
                break;
            case TOK_COMMA:
                bfcParseRead(compiler);
                break;
            case TOK_BRACE_LEFT:
                bfcParseConditional(compiler);
                break;
            case TOK_EOF:
            {
                const char *const progName = bfcGetProgramName(compiler->lexer);
                bfcPrintErrorPos(progName, openSrcPos.line, openSrcPos.column, "no matching ']'");
                bfcDefer(compiler);
            } return;
            default:
            {
                const char *const progName = bfcGetProgramName(compiler->lexer);
                const BFSourcePosition pos = bfcGetCurrentSourcePosition(compiler->lexer);
                bfcPrintErrorPos(progName, pos.line, pos.column, "unknown token: %c", (char)compiler->currToken);
                bfcDefer(compiler);
            } return;
        }
    }

    compiler->currToken = bfcNextToken(compiler->lexer);
    compiler->code[openPos].operands.instrLine = compiler->pos + 1;

    bfcEnsureCodeSpace(compiler);
    compiler->code[compiler->pos].instr = BFC_JMP;
    compiler->code[compiler->pos++].operands.instrLine = openPos;
}

static void bfcEnsureCodeSpace(BFCompiler *compiler)
{
    if (compiler->pos < compiler->size)
    {
        return;
    }

    compiler->size = compiler->size + (compiler->size / 2);
    compiler->code = BFC_REALLOC(BFOpCode, compiler->code, compiler->size);
}

static void bfcDefer(BFCompiler *compiler)
{
    BFC_FREE(compiler->code);
    compiler->code = NULL;
}
