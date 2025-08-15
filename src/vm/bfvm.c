#include "bfvm.h"

#include "core/error.h"
#include "core/memory.h"

#include <bfc/bfc.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BFVM_DATA_SIZE 30000

struct BFVirtualMachine
{
    u8               data[BFVM_DATA_SIZE];
    const BfcOpCode *code;
    size_t           ip;
    u16              dp;
};

static void bfvmAddb(BFVirtualMachine *vm, u8 val);
static void bfvmSubb(BFVirtualMachine *vm, u8 val);
static void bfvmAddp(BFVirtualMachine *vm, u16 val);
static void bfvmSubp(BFVirtualMachine *vm, u16 val);
static void bfvmWrite(BFVirtualMachine *vm);
static void bfvmRead(BFVirtualMachine *vm);
static void bfvmJz(BFVirtualMachine *vm, size_t line);
static void bfvmJmp(BFVirtualMachine *vm, size_t line);

BFVirtualMachine *bfvmInitVirtualMachine(int argc, char **argv)
{
    const BfcOpCode *const code = bfcCompile(argc < 2 ? NULL : argv[1]);
    if (!code)
    {
        return NULL;
    }

    BFVirtualMachine *const vm = BFVM_CALLOC(BFVirtualMachine, 1);
    vm->code = code;

    return vm;
}

void bfvmCloseVirtualMachine(BFVirtualMachine *vm)
{
    BFVM_FREE((void *)vm->code);
    BFVM_FREE(vm);
}

void bfvmRunVirtualMachine(BFVirtualMachine *vm)
{
    while (vm->code[vm->ip].instr != BFC_END)
    {
        switch (vm->code[vm->ip].instr)
        {
            case BFC_ADDB:
                bfvmAddb(vm, vm->code[vm->ip].operands.byteOffset);
                break;
            case BFC_SUBB:
                bfvmSubb(vm, vm->code[vm->ip].operands.byteOffset);
                break;
            case BFC_ADDP:
                bfvmAddp(vm, vm->code[vm->ip].operands.dataOffset);
                break;
            case BFC_SUBP:
                bfvmSubp(vm, vm->code[vm->ip].operands.dataOffset);
                break;
            case BFC_WRITE:
                bfvmWrite(vm);
                break;
            case BFC_READ:
                bfvmRead(vm);
                break;
            case BFC_JZ:
                bfvmJz(vm, vm->code[vm->ip].operands.instrLine);
                break;
            case BFC_JMP:
                bfvmJmp(vm, vm->code[vm->ip].operands.instrLine);
                break;
            default:
                bfvmPrintError("unknown instruction %d\n", vm->code[vm->ip].instr);
                break;
        }
    }
}

static void bfvmAddb(BFVirtualMachine *vm, u8 val)
{
    vm->data[vm->dp] += val;
    vm->ip++;
}

static void bfvmSubb(BFVirtualMachine *vm, u8 val)
{
    vm->data[vm->dp] -= val;
    vm->ip++;
}

static void bfvmAddp(BFVirtualMachine *vm, u16 val)
{
    vm->dp += val;
    if (vm->dp >= BFVM_DATA_SIZE)
    {
        bfvmPrintError("data pointer out of range");
    }
    
    vm->ip++;
}

static void bfvmSubp(BFVirtualMachine *vm, u16 val)
{
    vm->dp -= val;
    if (vm->dp >= BFVM_DATA_SIZE)
    {
        bfvmPrintError("data pointer out of range");
    }

    vm->ip++;
}

static void bfvmWrite(BFVirtualMachine *vm)
{
    if (putchar(vm->data[vm->dp]) == EOF)
    {
        bfvmPrintError("failed to output byte");
    }

    vm->ip++;
}

static void bfvmRead(BFVirtualMachine *vm)
{
    i32 ch = 0x00;
    if ((ch = fgetc(stdin)) == EOF)
    {
        bfvmPrintError("failed to read byte");
    }

    vm->data[vm->dp] = (u8)ch;
    vm->ip++;
}

static void bfvmJz(BFVirtualMachine *vm, size_t line)
{
    vm->ip = (vm->data[vm->dp] != 0) ? vm->ip + 1 : line;
}

static void bfvmJmp(BFVirtualMachine *vm, size_t line)
{
    vm->ip = line;
}

