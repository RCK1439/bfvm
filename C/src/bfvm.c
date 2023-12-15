/**
 * @file   bfvm.c
 * @brief  A virtual machine for the Brainfuck programming language
 * @author Ruan C. Keet
 * @date   25-10-2023
 */

#include "bfc.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* --- constants ------------------------------------------------------------*/

#define DATA_SIZE 30000

/* --- global variables -----------------------------------------------------*/

static u8 data[DATA_SIZE]; /* data array */
static u16 dp;             /* data ptr */
static size_t ip;          /* instruction ptr */

/* --- execution routines ---------------------------------------------------*/

static void addb(u8 val);
static void subb(u8 val);
static void addp(u16 val);
static void subp(u16 val);
static void write(void);
static void read(void);
static void jz(size_t line);
static void jmp(size_t line);

/* --- main routine ---------------------------------------------------------*/

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
int main(int argc, char *argv[])
{
    clock_t start, end;
    f32 delta;
    ByteCode *code;

    code = compile(argc < 2 ? NULL : argv[1]);
    ip = 0;

    start = clock();
    while (code[ip].op != BFVM_END) {
        switch (code[ip].op) {
            case BFVM_ADDB: addb(code[ip].bval); break;
            case BFVM_SUBB: subb(code[ip].bval); break;
            case BFVM_ADDP: addp(code[ip].dval); break;
            case BFVM_SUBP: subp(code[ip].dval); break;
            case BFVM_WRITE: write(); break;
            case BFVM_READ: read(); break;
            case BFVM_JZ: jz(code[ip].line); break;
            case BFVM_JMP: jmp(code[ip].line); break;
            default: log_err("unknown opcode %d\n", code[ip].op);
        }
    }
    end = clock();

    delta = (f32)(end - start) / CLOCKS_PER_SEC;
    log_norm("time elapsed: %.5fms", delta * 1000.0f);

    free(code);
    return EXIT_SUCCESS;
}

/* --- execution routines ---------------------------------------------------*/

inline static void addb(u8 val)
{
    data[dp] += val;
    ip++;
}

inline static void subb(u8 val)
{
    data[dp] -= val;
    ip++;
}

inline static void addp(u16 val)
{
    dp += val;
    if (dp >= DATA_SIZE) {
        log_err("data pointer out of range");
    }
    
    ip++;
}

inline static void subp(u16 val)
{
    dp -= val;
    if (dp >= DATA_SIZE) {
        log_err("data pointer out of range");
    }

    ip++;
}

inline static void write(void)
{
    if (putchar(data[dp]) == EOF) {
        log_err("failed to output byte");
    }

    ip++;
}

inline static void read(void)
{
    i32 ch;
    if ((ch = fgetc(stdin)) == EOF) {
        log_err("failed to read byte");
    }

    data[dp] = (u8)ch;
    ip++;
}

inline static void jz(size_t line)
{
    ip = (data[dp] != 0x00) ? ip + 1 : line;
}

inline static void jmp(size_t line)
{
    ip = line;
}
