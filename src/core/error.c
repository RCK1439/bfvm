#include "error.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define BFVM_ASCII_BOLD_CYAN  "\033[1;36m"
#define BFVM_ASCII_BOLD_WHITE "\033[1;37m"
#define BFVM_ASCII_BOLD_RED   "\033[1;31m"
#define BFVM_ASCII_RESET      "\033[m"

static void bfvmPrintInternal(FILE *stream, const char *prefix, const char *fmt, va_list args);

void bfvmPrintError(const char *fmt, ...)
{
    const char *prefix = BFVM_ASCII_BOLD_RED "error:" BFVM_ASCII_RESET;
    va_list args;

    va_start(args, fmt);
    bfvmPrintInternal(stderr, prefix, fmt, args);
    va_end(args);

    exit(EXIT_FAILURE);
}

void bfvmPanic(const char *fmt, ...)
{
    const char *prefix = BFVM_ASCII_BOLD_RED "fatal error:" BFVM_ASCII_RESET;
    va_list args;

    va_start(args, fmt);
    bfvmPrintInternal(stderr, prefix, fmt, args);
    va_end(args);

    exit(EXIT_FAILURE);
}

static void bfvmPrintInternal(FILE *stream, const char *prefix, const char *fmt, va_list args)
{
    fflush(stream);
    fprintf(stream, "%sbfvm:%s ", BFVM_ASCII_BOLD_WHITE, BFVM_ASCII_RESET);
    if (prefix != NULL)
    {
        fprintf(stream, "%s ", prefix);
    }
       
    vfprintf(stream, fmt, args);
    fprintf(stream, "\n");
}
