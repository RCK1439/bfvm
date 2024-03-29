/**
 * @file   bfc.h
 * @brief  Interface for the Brainfuck compiler for BFVM.
 * @author Ruan C. Keet
 * @date   03-11-2023
 */

#ifndef BFC_H
#define BFC_H

#include <stddef.h>
#include <stdint.h>

/* --- type definitions -----------------------------------------------------*/

/**
 * The following are opcodes for "bfvm assembly". These opcodes are
 * interpreted by the execution engine of bfvm. They work as follows:
 * 
 * addp <val>: adds <val> to the data pointer. Essentially moving the
 *             data pointer upwards by <val>.
 * 
 * subp <val>: subtracts <val> from the data pointer. Essentially moves
 *             the data pointer downwards by <val>.
 * 
 * addb <val>: adds <val> to the byte pointed to by the data pointer.
 * 
 * subb <val>: subtracts <val> from the byte pointed to by the
 *             data pointer.
 * 
 * write: writes the byte pointed to by the data pointer to stdout.
 * 
 * read: reads a byte in from stdin and saves it to the location
 *       pointed to by the data pointer.
 * 
 * jz <line>: jumps to the instruction at <line> if, and only if, the byte
 *            pointed to by the data pointer is zero.
 * 
 * jmp <line>: jumps to the instruction at <line>.
 * 
 * end: indicates the end of the program.
 */
typedef enum opcode_s {
    BFVM_ADDB,
    BFVM_SUBB,
    BFVM_ADDP,
    BFVM_SUBP,
    BFVM_WRITE,
    BFVM_READ,
    BFVM_JZ,
    BFVM_JMP,
    BFVM_END
} opcode_t;

/**
 * Combination of opcode and operand (if necessary).
 */
typedef struct bytecode_s {
    opcode_t op;    /* the opcode to execute */
    union {
        uint8_t bval;  /* the value to add to the byte */
        uint16_t dval; /* the offset on the data pointer */
        size_t line;   /* the instruction pointer to jump to */
    };
} bytecode_t;

/* --- bfc interface --------------------------------------------------------*/

/**
 * Compiles the Brainfuck source code to bfvm bytecode to be executed by the
 * virtual machine. If the filepath is NULL, it is assumed that the user
 * intends to read the source code from stdin.
 * 
 * @param[in] filepath
 *      The path to the source file of the Brainfuck program.
 * 
 * @return
 *      An array of instructions to be executed by the virtual machine.
 *      The array has a terminating value at the end (BFVM_END)
 */
bytecode_t *compile(const char *filepath);

#endif /* BFC_H */
