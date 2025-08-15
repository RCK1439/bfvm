#include "bfc.h"
#include "core/error.h"
#include "lexer.h"

#include "core/memory.h"

#define INIT_CODE_SIZE 32UL

#define PARSE_CHAIN(TOKEN, OP, OFFSET)                      \
    bfcEnsureCodeSpace(compiler);                           \
    compiler->code[compiler->pos].instr = (OP);             \
    OFFSET = 0;                                             \
    while (compiler->currToken == (TOKEN))                  \
    {                                                       \
        (OFFSET)++;                                         \
        bfcNextToken(compiler->lexer, &compiler->currToken);\
    }                                                       \
    compiler->pos++                                         \

typedef struct BfcCompiler
{
    BfcLexer  *lexer;
    BfcOpCode *code;
    size_t     pos;
    size_t     size;
    BfcToken   currToken;
} BfcCompiler;

static void bfcParseProgram(BfcCompiler *compiler);
static void bfcParseAddByte(BfcCompiler *compiler);
static void bfcParseSubByte(BfcCompiler *compiler);
static void bfcParseAddPtr(BfcCompiler *compiler);
static void bfcParseSubPtr(BfcCompiler *compiler);
static void bfcParseWrite(BfcCompiler *compiler);
static void bfcParseRead(BfcCompiler *compiler);
static void bfcParseConditional(BfcCompiler *compiler);

static void bfcEnsureCodeSpace(BfcCompiler *compiler);
static void bfcDefer(BfcCompiler *compiler);

const BfcOpCode *bfcCompile(const char *filepath)
{
    BfcLexer *const lexer = bfcInitLexer(filepath);
    if (!lexer)
    {
        return NULL;
    }

    BfcCompiler *const compiler = BFC_MALLOC(BfcCompiler, 1);
    compiler->lexer = lexer;
    compiler->code = BFC_MALLOC(BfcOpCode, INIT_CODE_SIZE);
    compiler->pos = 0;
    compiler->size = INIT_CODE_SIZE;

    bfcParseProgram(compiler);
    BfcOpCode *const code = compiler->code;

    BFC_FREE(compiler->code);
    bfcCloseLexer(compiler->lexer);
    BFC_FREE(compiler);

    return code;
}

/* --- parser routines ------------------------------------------------------*/

static void bfcParseProgram(BfcCompiler *compiler)
{
    bfcNextToken(compiler->lexer, &compiler->currToken);
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
                bfcPrintErrorPos(bfcGetErrorSink(compiler->lexer), "unknown command: %c", (char)compiler->currToken);
                bfcDefer(compiler);
                return;
        }
    }

    if (!compiler->code)
    {
        return;
    }

    bfcEnsureCodeSpace(compiler);
    compiler->code[compiler->pos].instr = BFC_END;
}

static void bfcParseAddByte(BfcCompiler *compiler)
{
    if (!compiler->code)
    {
        return;
    }

    PARSE_CHAIN(TOK_ADD, BFC_ADDB, compiler->code[compiler->pos].operands.byteOffset);
}

static void bfcParseSubByte(BfcCompiler *compiler)
{
    if (!compiler->code)
    {
        return;
    }

    PARSE_CHAIN(TOK_SUB, BFC_SUBB, compiler->code[compiler->pos].operands.byteOffset);
}

static void bfcParseAddPtr(BfcCompiler *compiler)
{
    if (!compiler->code)
    {
        return;
    }

    PARSE_CHAIN(TOK_ARROW_RIGHT, BFC_ADDP, compiler->code[compiler->pos].operands.dataOffset);
}

static void bfcParseSubPtr(BfcCompiler *compiler)
{
    if (!compiler->code)
    {
        return;
    }

    PARSE_CHAIN(TOK_ARROW_LEFT, BFC_SUBP, compiler->code[compiler->pos].operands.dataOffset);
}

static void bfcParseWrite(BfcCompiler *compiler)
{
    if (!compiler->code)
    {
        return;
    }

    bfcEnsureCodeSpace(compiler);

    compiler->code[compiler->pos++].instr = BFC_WRITE;
    bfcNextToken(compiler->lexer, &compiler->currToken);
}

static void bfcParseRead(BfcCompiler *compiler)
{
    if (!compiler->code)
    {
        return;
    }

    bfcEnsureCodeSpace(compiler);

    compiler->code[compiler->pos++].instr = BFC_READ;
    bfcNextToken(compiler->lexer, &compiler->currToken);
}

static void bfcParseConditional(BfcCompiler *compiler)
{
    if (!compiler->code)
    {
        return;
    }

    const size_t openPos = compiler->pos;
    const BfcSourcePosition openSrcPosition = bfcGetCurrentSourcePosition(compiler->lexer);

    bfcEnsureCodeSpace(compiler);
    compiler->code[compiler->pos++].instr = BFC_JZ;

    bfcNextToken(compiler->lexer, &compiler->currToken);
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
                BfcErrorSink sink = bfcGetErrorSink(compiler->lexer);
                sink.position = openSrcPosition;
                bfcPrintErrorPos(sink, "no matching ']'");
                bfcDefer(compiler);
            } return;
            default:
                bfcPrintErrorPos(bfcGetErrorSink(compiler->lexer), "unknown command: %c", (char)compiler->currToken);
                bfcDefer(compiler);
                return;
        }
    }

    bfcNextToken(compiler->lexer, &compiler->currToken);
    compiler->code[openPos].operands.instrLine = compiler->pos + 1;

    bfcEnsureCodeSpace(compiler);
    compiler->code[compiler->pos].instr = BFC_JMP;
    compiler->code[compiler->pos++].operands.instrLine = openPos;
}

static void bfcEnsureCodeSpace(BfcCompiler *compiler)
{
    if (compiler->pos < compiler->size)
    {
        return;
    }

    compiler->size = compiler->size + (compiler->size / 2);
    compiler->code = BFC_REALLOC(BfcOpCode, compiler->code, compiler->size);
}

static void bfcDefer(BfcCompiler *compiler)
{
    BFC_FREE(compiler->code);
    compiler->code = NULL;
}
