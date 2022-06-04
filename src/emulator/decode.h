#ifndef DECODE
#define DECODE

#include "../definitions.h"
#include "stdbool.h"

typedef enum InstructionType { 
    DPI, 
    MULT, 
    BR, 
    SDTI 
} InstructionType;

typedef struct ShiftInstruction {
    word result;
    unsigned int carry;
} ShiftInstruction;

typedef struct DataProcessingInstruction {
    word cond;
    unsigned int i;
    Opcode opcode;
    unsigned int s;
    unsigned int rn;
    unsigned int rd;
    word rotate_shift;
    word op2;
} DataProcessingInstruction;

typedef struct MultiplyInstruction {
    word cond;
    unsigned int a;
    unsigned int s;
    unsigned int rd;
    unsigned int rn;
    unsigned int rs;
    unsigned int rm;
} MultiplyInstruction;

typedef struct SingleDataTransferInstruction {
    word cond;
    unsigned int i;
    unsigned int p;
    unsigned int u;
    unsigned int l;
    unsigned int rn;
    unsigned int rd;
    word offset;
} SingleDataTransferInstruction;

typedef struct BranchInstruction {
    word cond;
    unsigned int offset;
} BranchInstruction;

typedef union DecodedInstruction {
    InstructionType type;
    word instruction;
    DataProcessingInstruction *dp;
    MultiplyInstruction *multiply;
    SingleDataTransferInstruction *sdt;
    BranchInstruction *branch;
} DecodedInstruction;

#endif