#ifndef BFC_H
#define BFC_H

#include "core/types.h"

typedef enum BfcInstr
{
    BFC_ADDB,
    BFC_SUBB,
    BFC_ADDP,
    BFC_SUBP,
    BFC_WRITE,
    BFC_READ,
    BFC_JZ,
    BFC_JMP,
    BFC_END
} BfcInstr;

typedef union BfcOperand
{
    size_t instrLine;
    u16    dataOffset;
    u8     byteOffset;
} BfcOperand;

typedef struct BfcOpCode
{
    BfcOperand operands;
    BfcInstr   instr;
} BfcOpCode;

const BfcOpCode *bfcCompile(const char *filepath);

#endif /* BFC_H */
