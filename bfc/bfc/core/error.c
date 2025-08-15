#include "error.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BFC_FMT_PROG    "%s%s%s:%s%zu:%zu%s: %s"
#define BFC_FMT_NO_PROG "%s%zu:%zu%s: %s"

#define BFC_ASCII_BOLD_CYAN  "\033[1;36m"
#define BFC_ASCII_BOLD_WHITE "\033[1;37m"
#define BFC_ASCII_BOLD_RED   "\033[1;31m"
#define BFC_ASCII_RESET      "\033[m"

#define BFC_MAX_PREFIX_SIZE 512

static void bfcPrintInternal(FILE *out, const char *prefix, const char *fmt, va_list args);

void bfcPrintNormal(const char *fmt, ...)
{
    va_list args = { 0 };

    va_start(args, fmt);
    bfcPrintInternal(stdout, NULL, fmt, args);
    va_end(args);
}

void bfcPrintInfo(const char *fmt, ...)
{
    const char *prefix = BFC_ASCII_BOLD_CYAN "info:" BFC_ASCII_RESET;
    va_list args = { 0 };

    va_start(args, fmt);
    bfcPrintInternal(stdout, prefix, fmt, args);
    va_end(args);
}

void bfcPrintError(const char *fmt, ...)
{
    const char *prefix = BFC_ASCII_BOLD_RED "error:" BFC_ASCII_RESET;
    va_list args = { 0 };

    va_start(args, fmt);
    bfcPrintInternal(stderr, prefix, fmt, args);
    va_end(args);
}

void bfcPrintErrorPos(const char *progName, size_t line, size_t column, const char *fmt, ...)
{
    const char *err = BFC_ASCII_BOLD_RED "error:" BFC_ASCII_RESET;    
    char prefix[BFC_MAX_PREFIX_SIZE+1] = { 0 };
    va_list args = { 0 };

    if (progName)
    {
        sprintf(prefix, BFC_FMT_PROG,
            BFC_ASCII_BOLD_WHITE, progName, BFC_ASCII_RESET,
            BFC_ASCII_BOLD_WHITE, line, column, BFC_ASCII_RESET,
            err
        );
    }
    else
    {
        sprintf(prefix, BFC_FMT_NO_PROG,
            BFC_ASCII_BOLD_WHITE, line, column, BFC_ASCII_RESET,
            err
        );
    }

    va_start(args, fmt);
    bfcPrintInternal(stderr, prefix, fmt, args);
    va_end(args);
}

void bfcPanic(const char *fmt, ...)
{
    va_list args = { 0 };

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    exit(EXIT_FAILURE);
}

static void bfcPrintInternal(FILE *out, const char *prefix, const char *fmt, va_list args)
{
    fflush(out);
    fprintf(out, "%sbfc:%s", BFC_ASCII_BOLD_WHITE, BFC_ASCII_RESET);
    if (prefix)
    {
        fprintf(out, "%s", prefix);
    }

    vfprintf(out, fmt, args);
    fputc('\n', out);
}
