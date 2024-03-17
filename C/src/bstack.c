/**
 * @file   stack.c
 * @brief  Implementation of the brace stack.
 * @author Ruan C. Keet
 * @date   25-10-2023
 */

#include "bstack.h"
#include "error.h"

#include <stdlib.h>

/* --- constants ------------------------------------------------------------*/

#define INIT_SIZE 32

/* --- global variables -----------------------------------------------------*/

static Brace *stack; /* the stack */
static size_t sp;    /* the stack pointer */
static size_t size;  /* the current size of the stack */

/* --- function prototypes --------------------------------------------------*/

/**
 * Ensures that there is enough space in the stack for a new element to be
 * pushed on.
 */
static void ensure_space(void);

/* --- bstack interface ------------------------------------------------------*/

void init_stack(void)
{
    size = INIT_SIZE;
    stack = (Brace*)emalloc(sizeof(Brace) * size);

    sp = 0;
}

void free_stack(void)
{
    free(stack);
}

void push(Brace val)
{
    ensure_space();
    stack[sp++] = val;
}

void pop(Brace *out)
{
    if (empty()) {
        log_err("stack underflow");
    }

    *out = stack[--sp];
}

bool empty(void)
{
    return sp == 0;
}

/* --- utility functions ----------------------------------------------------*/

static void ensure_space(void)
{
    if (sp < size) {
        return;
    }

    size = size + (size / 2);
    stack = (Brace*)erealloc(stack, sizeof(Brace) * size);
}
