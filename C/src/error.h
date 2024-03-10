/**
 * @file   error.h
 * @brief  Interface definition for error handling functions in BFVM.
 * @author Ruan C. Keet
 * @date   03-11-2023
 */

#ifndef ERROR_H
#define ERROR_H

#include <stddef.h>

/* --- type definitions -----------------------------------------------------*/

/**
 * The current position in the source file.
 */
typedef struct SourcePosition {
    size_t line;   /* the current line in the source file */
    size_t column; /* the column of the current character */
} SourcePosition;

extern SourcePosition position; /* the position in the source file being
                                   compiled */

/* --- error interface ------------------------------------------------------*/

/**
 * Sets the program name for the error system. This will be used to indicate
 * errors, if any, in the source file during compilation.
 * 
 * @param[in] filename
 *      The name of the source file.
 */
void setprogname(const char *filename);

/**
 * Frees the program name from memory.
 */
void freeprogname(void);

/**
 * Logs the given text to stdout as normal with a new line at the end.
 * The text will look like: "bfvm: <text>"
 * 
 * @param[in] fmt
 *      The text format.
 */
void log_norm(const char *fmt, ...);

/**
 * Logs the given text to stdout with an info tag at the start and a newline
 * at the end. The text will look like: "bfvm: info: <text>"
 * 
 * @param[in] fmt
 *      The text format.
 */
void log_info(const char *fmt, ...);

/**
 * Logs the given text to stderr with an error tag at the start and a newline
 * at the end. This function terminates the virtual machine with EXIT_FAILURE.
 * The text will look like: "bfvm: error: <text>"
 * 
 * @param[in] fmt
 *      The text format.
 */
void log_err(const char *fmt, ...);

/**
 * Logs the given text to stderr with an error tag at the start and a newline
 * at the end. This also provides extra information like the position of the
 * error and the source file name. This function is to be used during
 * compilation. This function terminates the virtual machine with
 * EXIT_FAILURE.
 * The text will look like: "bfvm: <filename>:<line>:<col>: error: <text>"
 * 
 * @param[in] fmt
 *      The text format.
 */
void log_errpos(const char *fmt, ...);

/**
 * Attempts to allocate up to size bytes of memory. Terminates the virtual
 * machine if allocation fails.
 * 
 * @param[in] size
 *      The amount of bytes to allocate.
 * 
 * @return
 *      A pointer to the allocated memory.
 */
void *emalloc(size_t size);

/**
 * Attempts to reallocate the given pointer with a new size. Terminates
 * the virtual machine if reallocation fails.
 * 
 * @param[in] p
 *      The pointer to the memory to reallocate.
 * @param[in] size
 *      The new size of the memory space.
 * 
 * @return
 *      The pointer to the reallocated memory.
 */
void *erealloc(void *p, size_t size);

/**
 * Duplicates the given string in memory.
 * 
 * @param[in] s
 *      The string to duplicate.
 * 
 * @return
 *      A duplicate of the string.
 */
char *estrdup(const char *s);

#endif /* ERROR_H */
