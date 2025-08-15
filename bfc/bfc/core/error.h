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

typedef struct BFSourcePosition
{
    size_t line;
    size_t column;
} BFSourcePosition;

typedef struct BFErrorSink
{
    char            *programName;
    BFSourcePosition position;
} BFErrorSink;

BFErrorSink bfcInitErrorSink(const char *filename);
void bfcCloseErrorSink(BFErrorSink sink);

void bfcPrintNormal(BFErrorSink sink, const char *fmt, ...);
void bfcPrintInfo(BFErrorSink sink, const char *fmt, ...);
void bfcPrintError(BFErrorSink sink, const char *fmt, ...);
void bfcPrintErrorPos(BFErrorSink sink, const char *fmt, ...);

void bfcPanic(const char *fmt, ...);

#endif /* ERROR_H */
