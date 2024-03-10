/**
 * @file   error.c
 * @brief  Implementation of the error handling functions in BFVM.
 * @author Ruan C. Keet
 * @date   03-11-2023
 */

#include "error.h"

#include <assert.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* --- ASCII colors ---------------------------------------------------------*/

#define ESC                      "\033["
#define BOLD                     "1;"
#define ASCII_RESET              ESC "m"

#define BLACK                    "30m"
#define RED                      "31m"
#define GREEN                    "32m"
#define YELLOW                   "33m"
#define BLUE                     "34m"
#define MAGENTA                  "35m"
#define CYAN                     "36m"
#define WHITE                    "37m"

#define HIGH_BLACK               "90m"
#define HIGH_RED                 "91m"
#define HIGH_GREEN               "92m"
#define HIGH_YELLOW              "93m"
#define HIGH_BLUE                "94m"
#define HIGH_MAGENTA             "95m"
#define HIGH_CYAN                "96m"
#define HIGH_WHITE               "97m"

#define ASCII_BLACK              ESC BLACK
#define ASCII_RED                ESC RED
#define ASCII_GREEN              ESC GREEN
#define ASCII_YELLOW             ESC YELLOW
#define ASCII_BLUE               ESC BLUE
#define ASCII_MAGENTA            ESC MAGENTA
#define ASCII_CYAN               ESC CYAN
#define ASCII_WHITE              ESC WHITE

#define ASCII_BOLD_BLACK         ESC BOLD BLACK
#define ASCII_BOLD_RED           ESC BOLD RED
#define ASCII_BOLD_GREEN         ESC BOLD GREEN
#define ASCII_BOLD_YELLOW        ESC BOLD YELLOW
#define ASCII_BOLD_BLUE          ESC BOLD BLUE
#define ASCII_BOLD_MAGENTA       ESC BOLD MAGENTA
#define ASCII_BOLD_CYAN          ESC BOLD CYAN
#define ASCII_BOLD_WHITE         ESC BOLD WHITE

#define ASCII_HIGH_BLACK         ESC HIGH_BLACK
#define ASCII_HIGH_RED           ESC HIGH_RED
#define ASCII_HIGH_GREEN         ESC HIGH_GREEN
#define ASCII_HIGH_YELLOW        ESC HIGH_YELLOW
#define ASCII_HIGH_BLUE          ESC HIGH_BLUE
#define ASCII_HIGH_MAGENTA       ESC HIGH_MAGENTA
#define ASCII_HIGH_CYAN          ESC HIGH_CYAN
#define ASCII_HIGH_WHITE         ESC HIGH_WHITE

#define ASCII_BOLD_HIGH_BLACK    ESC BOLD HIGH_BLACK
#define ASCII_BOLD_HIGH_RED      ESC BOLD HIGH_RED
#define ASCII_BOLD_HIGH_GREEN    ESC BOLD HIGH_GREEN
#define ASCII_BOLD_HIGH_YELLOW   ESC BOLD HIGH_YELLOW
#define ASCII_BOLD_HIGH_BLUE     ESC BOLD HIGH_BLUE
#define ASCII_BOLD_HIGH_MAGENTA  ESC BOLD HIGH_MAGENTA
#define ASCII_BOLD_HIGH_CYAN     ESC BOLD HIGH_CYAN
#define ASCII_BOLD_HIGH_WHITE    ESC BOLD HIGH_WHITE

/* --- global variables -----------------------------------------------------*/

SourcePosition position; /* the current position in the source file being
                            compiled */
static char *progname;   /* the name of the program being compiled */

/* --- function prototypes --------------------------------------------------*/

/**
 * Prints the arguments as a formatted string to the output stream.
 * 
 * @param[in] stream
 *      The stream to print to.
 * @param[in] prefix
 *      The prefix to the string.
 * @param[in] fmt
 *      The format of the string.
 * @param[in] args
 *      The arguments passed in to print out.
 */
static void cprintf(FILE *stream, const char *prefix, const char *fmt,
    va_list args);

/* --- error interface ------------------------------------------------------*/

void setprogname(const char *filename)
{
    char *c;

    if ((c = strrchr(filename, '/')) == NULL) {
        c = (char*)filename;
    } else {
        c++;
    }

    progname = estrdup(c);
}

void freeprogname(void)
{
    free(progname);
}

void log_norm(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    cprintf(stdout, NULL, fmt, args);
    va_end(args);
}

void log_info(const char *fmt, ...)
{
    int istty = isatty(2);
    va_list args;
    const char *prefix = istty ? ASCII_BOLD_CYAN "info:" ASCII_RESET : "info:";

    va_start(args, fmt);
    cprintf(stdout, prefix, fmt, args);
    va_end(args);
}

void log_err(const char *fmt, ...)
{
    int istty = isatty(2);
    va_list args;
    const char *prefix = istty ? ASCII_BOLD_RED "error:" ASCII_RESET :
        "error:";

    va_start(args, fmt);
    cprintf(stderr, prefix, fmt, args);
    va_end(args);

    exit(EXIT_FAILURE);
}

void log_errpos(const char *fmt, ...)
{
    int istty = isatty(2);
    va_list args;
    char prefix[512];
    const char *start = istty ? ASCII_BOLD_WHITE : "";
    const char *end = istty ? ASCII_RESET : "";
    const char *err = istty ? ASCII_BOLD_RED "error:" ASCII_RESET : "error:";

    if (progname) {
        snprintf(prefix, 512, "%s%s%s:%s%lu:%lu%s: %s",
            start, progname, end,
            start, position.line, position.column, end,
            err);
    } else {
        snprintf(prefix, 512, "%s%lu:%lu%s: %s",
            start, position.line, position.column, end,
            err);
    }

    va_start(args, fmt);
    cprintf(stderr, prefix, fmt, args);
    va_end(args);

    exit(EXIT_FAILURE);
}

void *emalloc(size_t size)
{
    void *ptr;

    if ((ptr = malloc(size)) == NULL) {
        log_err("out of memory");
    }

    return ptr;
}

void *erealloc(void *p, size_t size)
{
    void *ptr;

    if ((ptr = realloc(p, size)) == NULL) {
        log_err("out of memory");
    }

    return ptr;
}

char *estrdup(const char *s)
{
    char *dup;
    size_t size;

    size = strlen(s) + 1;
    if ((dup = (char*)malloc(sizeof(char) * size)) == NULL) {
        log_err("failed to duplicate string: out of memory");
    }

    strncpy(dup, s, size - 1);
    return dup;
}

/* --- utility functions ----------------------------------------------------*/

static void cprintf(FILE *stream, const char *prefix, const char *fmt,
    va_list args)
{
    int istty = isatty(2);
    const char *ac_start = (istty ? ASCII_BOLD_WHITE : "");
    const char *ac_end = (istty ? ASCII_RESET : "");

    fflush(stdout);
    fprintf(stream, "%sbfvm:%s ", ac_start, ac_end);
    if (prefix != NULL) {
        fprintf(stream, "%s ", prefix);
    }
       
    vfprintf(stream, fmt, args);
    fprintf(stream, "\n");
}
