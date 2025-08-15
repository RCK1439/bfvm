#include "error.h"
#include "memory.h"

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

BFErrorSink bfcInitErrorSink(const char *filename)
{
    BFErrorSink sink = { 0 };

    if (filename)
    {
        char *c = strchr(filename, '/');
        if (!c)
        {
            c = (char *)filename;
        }
        else
        {
            c++;
        }
        
        sink.programName = bfcCloneString(c);
    }
    else
    {
        sink.programName = NULL;
    }

    sink.position.line = 1;
    sink.position.column = 0;

    return sink;
}

void bfcCloseErrorSink(BFErrorSink sink)
{
    BFC_FREE(sink.programName);
}

void bfcPrintNormal(BFErrorSink sink, const char *fmt, ...)
{
    (void)sink; // Unused

    va_list args = { 0 };

    va_start(args, fmt);
    bfcPrintInternal(stdout, NULL, fmt, args);
    va_end(args);
}

void bfcPrintInfo(BFErrorSink sink, const char *fmt, ...)
{
    (void)sink; // Unused

    const char *prefix = BFC_ASCII_BOLD_CYAN "info:" BFC_ASCII_RESET;
    va_list args = { 0 };

    va_start(args, fmt);
    bfcPrintInternal(stdout, prefix, fmt, args);
    va_end(args);
}

void bfcPrintError(BFErrorSink sink, const char *fmt, ...)
{
    (void)sink; // Unused

    const char *prefix = BFC_ASCII_BOLD_RED "error:" BFC_ASCII_RESET;
    va_list args = { 0 };

    va_start(args, fmt);
    bfcPrintInternal(stderr, prefix, fmt, args);
    va_end(args);
}

void bfcPrintErrorPos(BFErrorSink sink, const char *fmt, ...)
{
    const char *err = BFC_ASCII_BOLD_RED "error:" BFC_ASCII_RESET;    
    char prefix[BFC_MAX_PREFIX_SIZE+1] = { 0 };
    va_list args = { 0 };

    if (sink.programName)
    {
        sprintf(prefix, BFC_FMT_PROG,
            BFC_ASCII_BOLD_WHITE, sink.programName, BFC_ASCII_RESET,
            BFC_ASCII_BOLD_WHITE, sink.position.line, sink.position.column, BFC_ASCII_RESET,
            err
        );
    }
    else
    {
        sprintf(prefix, BFC_FMT_NO_PROG,
            BFC_ASCII_BOLD_WHITE, sink.position.line, sink.position.column, BFC_ASCII_RESET,
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
