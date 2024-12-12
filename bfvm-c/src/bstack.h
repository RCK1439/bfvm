/**
 * @file   stack.h
 * @brief  Definition for the brace stack interface.
 * @author Ruan C. Keet
 * @date   25-10-2023
 */

#ifndef BSTACK_H
#define BSTACK_H

#include "error.h"
#include "types.h"

/* --- type definitions -----------------------------------------------------*/

/**
 * The position of the brace, both in the assembly code and in the source
 * file.
 */
typedef struct brace_s {
    size_t asm_pos;      /* the position in the assembly code */
    sourcepos_t src_pos; /* the position in the source file */
} brace_t;

/* --- bstack interface ------------------------------------------------------*/

/**
 * Initializes and allocates the stack.
 */
void init_stack(void);

/**
 * Frees all resources used by the stack.
 */
void free_stack(void);

/**
 * Pushes the brace position ontop of the stack.
 * 
 * @param[in] b
 *      The brace position to push onto the stack.
 */
void push(brace_t b);

/**
 * Pops the brace position from the top of the stack off of the stack.
 * 
 * @param[out] out
 *      The position on the top of the stack.
 */
brace_t pop(void);

/**
 * Checks if the stack has any elements inside the container.
 * 
 * @return
 *      `true`, if the stack is empty.
 */
bool empty(void);

#endif /* BSTACK_H */
