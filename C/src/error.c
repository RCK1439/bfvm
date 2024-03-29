/**
 * @file   error.c
 * @brief  Implementation of the error handling functions in BFVM.
 * @author Ruan C. Keet
 * @date   03-11-2023
 */

#include "error.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* --- ASCII colors ---------------------------------------------------------*/

#define ASCII_BOLD_CYAN "\033[1;36m"
#define ASCII_BOLD_WHITE "\033[1;37m"
#define ASCII_BOLD_RED "\033[1;31m"

#define ASCII_RESET "\033[m"

/* --- global variables -----------------------------------------------------*/

sourcepos_t position;  /* the current position in the source file being
                          compiled */
static char *progname; /* the name of the program being compiled */

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
    const char *prefix = ASCII_BOLD_CYAN "info:" ASCII_RESET;
    va_list args;

    va_start(args, fmt);
    cprintf(stdout, prefix, fmt, args);
    va_end(args);
}

void log_err(const char *fmt, ...)
{
    const char *prefix = ASCII_BOLD_RED "error:" ASCII_RESET;
    va_list args;

    va_start(args, fmt);
    cprintf(stderr, prefix, fmt, args);
    va_end(args);

    exit(EXIT_FAILURE);
}

void log_errpos(const char *fmt, ...)
{
    #define MAX_PREFIX_SIZE 512

    const char *err = ASCII_BOLD_RED "error:" ASCII_RESET;
    va_list args;
    char prefix[MAX_PREFIX_SIZE];

    if (progname) {
        snprintf(prefix, MAX_PREFIX_SIZE, "%s%s%s:%s%lu:%lu%s: %s",
            ASCII_BOLD_WHITE, progname, ASCII_RESET,
            ASCII_BOLD_WHITE, position.line, position.column, ASCII_RESET,
            err);
    } else {
        snprintf(prefix, MAX_PREFIX_SIZE, "%s%lu:%lu%s: %s",
            ASCII_BOLD_WHITE, position.line, position.column, ASCII_RESET,
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
    fflush(stdout);
    fprintf(stream, "%sbfvm:%s ", ASCII_BOLD_WHITE, ASCII_RESET);
    if (prefix != NULL) {
        fprintf(stream, "%s ", prefix);
    }
       
    vfprintf(stream, fmt, args);
    fprintf(stream, "\n");
}
