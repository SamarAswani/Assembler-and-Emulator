#ifndef DEFINITIONS
#define DEFINITIONS

#include <stdio.h>

#define NUM_REGISTERS 17
#define MEM_SIZE 65536
#define WORD_IN_BYTES 4

typedef u_int32_t word;
typedef u_int8_t byte;
typedef u_int16_t address;

typedef struct{
    word registers[NUM_REGISTERS];
    byte memory[MEM_SIZE];
    word fetched;
    // decoded instruction
} State;

typedef enum { 
    SP = 13,
    LR = 14, 
    PC = 15, 
    CPSR = 16
} Register;

typedef enum {
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

typedef enum {
    EQ = 0,
    NE = 1,
    GE = 10,
    LT = 11,
    GT = 12,
    LE = 13,
    AL = 14
} Cond;

typedef enum {
    LSL = 0,
    LSR = 1,
    ASR = 2,
    ROR = 3
} Shift;

typedef enum { 
    MUL, 
    MLA 
} OpcodeMultiply;

typedef enum { 
    LDR, 
    STR 
} OpcodeSDT;

typedef enum { 
    BEQ, 
    BNE, 
    BGE, 
    BLT, 
    BGT, 
    BLE, 
    B 
} OpcodeBranch;

#endif