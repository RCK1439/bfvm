#ifndef BFC_H
#define BFC_H

#include "core/types.h"

typedef enum BFInstr
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
} BFInstr;

typedef union BFOperand
{
    size_t instrLine;
    u16    dataOffset;
    u8     byteOffset;
} BFOperand;

typedef struct BFOpCode
{
    BFOperand operands;
    BFInstr   instr;
} BFOpCode;

const BFOpCode *bfcCompile(const char *filepath);

#endif /* BFC_H */
