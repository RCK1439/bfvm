/**
 * @file   bfvm.c
 * @brief  A virtual machine for the Brainfuck programming language
 * @author Ruan C. Keet
 * @date   25-10-2023
 */

#include "bfc/bfc.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* --- constants ----------------------------------------------------------- */

#define DATA_SIZE 30000UL

/* --- global variables ---------------------------------------------------- */

static uint8_t data[DATA_SIZE]; /* data array */
static uint16_t dp;             /* data ptr */
static size_t ip;               /* instruction ptr */

/* --- execution routines -------------------------------------------------- */

static void addb(uint8_t val);
static void subb(uint8_t val);
static void addp(uint16_t val);
static void subp(uint16_t val);
static void write(void);
static void read(void);
static void jz(size_t line);
static void jmp(size_t line);

/* --- main routine -------------------------------------------------------- */

/**
 * The entry point to the program.
 * 
 * @param[in] argc
 *      The amount of arguments passed via the CLI.
 * @param[in] argv
 *      The arguments passed via the CLI.
 * 
 * @return
 *      The exit status of the program.
 */
int main(int argc, char *argv[]) {
    const bytecode_t *const code = compile(argc < 2 ? NULL : argv[1]);
    ip = 0;

    while (code[ip].op != BFVM_END) {
        switch (code[ip].op) {
            case BFVM_ADDB: addb(code[ip].operands.byte_offset); break;
            case BFVM_SUBB: subb(code[ip].operands.byte_offset); break;
            case BFVM_ADDP: addp(code[ip].operands.data_offset); break;
            case BFVM_SUBP: subp(code[ip].operands.data_offset); break;
            case BFVM_WRITE: write(); break;
            case BFVM_READ: read(); break;
            case BFVM_JZ: jz(code[ip].operands.instr_line); break;
            case BFVM_JMP: jmp(code[ip].operands.instr_line); break;
            default: log_err("unknown opcode %d\n", code[ip].op);
        }
    }

    BFVM_FREE(code);
    return EXIT_SUCCESS;
}

/* --- execution routines ---------------------------------------------------*/

static void addb(uint8_t val) {
    data[dp] += val;
    ip++;
}

static void subb(uint8_t val) {
    data[dp] -= val;
    ip++;
}

static void addp(uint16_t val) {
    dp += val;
    if (dp >= DATA_SIZE) {
        log_err("data pointer out of range");
    }
    
    ip++;
}

static void subp(uint16_t val) {
    dp -= val;
    if (dp >= DATA_SIZE) {
        log_err("data pointer out of range");
    }

    ip++;
}

static void write(void) {
    if (putchar(data[dp]) == EOF) {
        log_err("failed to output byte");
    }

    ip++;
}

static void read(void) {
    int32_t ch;

    if ((ch = fgetc(stdin)) == EOF) {
        log_err("failed to read byte");
    }

    data[dp] = (uint8_t)ch;
    ip++;
}

static void jz(size_t line) {
    ip = (data[dp] != 0) ? ip + 1 : line;
}

static void jmp(size_t line) {
    ip = line;
}

