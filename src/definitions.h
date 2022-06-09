#ifndef DEFINITIONS
#define DEFINITIONS

#include <stdio.h>
#include <stdint.h>

#define NUM_REGISTERS 17
#define MEM_SIZE 65536
#define WORD_IN_BYTES 4
#define WORD_IN_BITS 32
#define BYTE 8

typedef uint32_t word;
typedef uint8_t byte;
typedef uint16_t address;

typedef enum Register { 
    SP = 13,
    LR = 14, 
    PC = 15, 
    CPSR = 16
} Register;

typedef enum Opcode {
    AND = 0,
    EOR = 1,
    SUB = 2,
    RSB = 3,
    ADD = 4,
    TST = 8,
    TEQ = 9,
    CMP = 10,
    ORR = 12,
    MOV = 13
} Opcode;

typedef enum Cond {
    EQ = 0,
    NE = 1,
    GE = 10,
    LT = 11,
    GT = 12,
    LE = 13,
    AL = 14
} Cond;

typedef enum Shift {
    LSL = 0,
    LSR = 1,
    ASR = 2,
    ROR = 3
} Shift;

typedef enum OpcodeMultiply { 
    MUL, 
    MLA 
} OpcodeMultiply;

typedef enum OpcodeSDT { 
    LDR, 
    STR 
} OpcodeSDT;

typedef enum OpcodeBranch { 
    BEQ, 
    BNE, 
    BGE, 
    BLT, 
    BGT, 
    BLE, 
    B 
} OpcodeBranch;

typedef enum InstructionType { 
    DPI, 
    MULT, 
    BR, 
    SDTI 
} InstructionType;

typedef struct {
    word result;
    unsigned int carry;
} OperationInstruction;

typedef struct DataProcessingInstruction {
    InstructionType type;
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
    InstructionType type;
    word cond;
    unsigned int a;
    unsigned int s;
    int rd;
    int rn;
    int rs;
    int rm;
} MultiplyInstruction;

typedef struct SingleDataTransferInstruction {
    InstructionType type;
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
    int offset;
} BranchInstruction;

typedef union DecodedInstruction {
    InstructionType type;
    word instruction;
    DataProcessingInstruction dp;
    MultiplyInstruction multiply;
    SingleDataTransferInstruction sdt;
    BranchInstruction branch;
} DecodedInstruction;

typedef struct State {
    word registers[NUM_REGISTERS];
    byte memory[MEM_SIZE];
    word fetched;
    DecodedInstruction decoded;
} State;

#endif
