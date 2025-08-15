#ifndef ERROR_H
#define ERROR_H

#include "platform.h"

#if defined(BFVM_DEBUG)
#   define BFVM_ASSERT(expr, ...) if (!(expr)) bfvmPanic("Assertion failed: " __VA_ARGS__)
#else
#   define BFVM_ASSERT(expr, ...) (void)0
#endif

void bfvmPrintError(const char *fmt, ...);
void bfvmPanic(const char *fmt, ...);

#endif /* ERROR_H */
