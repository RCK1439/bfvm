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

void bfcPrintNormal(const char *fmt, ...);
void bfcPrintInfo(const char *fmt, ...);
void bfcPrintError(const char *fmt, ...);
void bfcPrintErrorPos(const char *progName, size_t line, size_t column, const char *fmt, ...);

void bfcPanic(const char *fmt, ...);

#endif /* ERROR_H */
