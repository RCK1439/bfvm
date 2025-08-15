#ifndef ERROR_H
#define ERROR_H

#include "platform.h"
#include "types.h"

#if defined(BFC_DEBUG)
#   define BFC_ASSERT(expr, ...)                  \
    if (!(expr))                                  \
        bfcPanic("Assertion failed: " __VA_ARGS__)\

#else
#   define BFC_ASSERT(expr, ...) (void)0
#endif

typedef struct BfcSourcePosition
{
    size_t line;
    size_t column;
} BfcSourcePosition;

typedef struct BfcErrorSink
{
    char             *programName;
    BfcSourcePosition position;
} BfcErrorSink;

BfcErrorSink bfcInitErrorSink(const char *filename);
void bfcCloseErrorSink(BfcErrorSink sink);

void bfcPrintNormal(BfcErrorSink sink, const char *fmt, ...);
void bfcPrintInfo(BfcErrorSink sink, const char *fmt, ...);
void bfcPrintError(BfcErrorSink sink, const char *fmt, ...);
void bfcPrintErrorPos(BfcErrorSink sink, const char *fmt, ...);

void bfcPanic(const char *fmt, ...);

#endif /* ERROR_H */
