#ifndef ASSEMBLER_FUNCTIONS
#define ASSEMBLER_FUNCTIONS

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "constants.h"
#include "loadArmLines.h"
#include "symbolTable.h"

#include <stdio.h>

typedef enum SDTIAddressType {
    NUMERIC_CONST, 
    PRE_IDX, 
    PRE_IDX_EXP, 
    POST_IDX_EXP
} SDTIAddressType;

typedef struct tableStruct {
    char *key;
    word value;
} tableStruct;

typedef struct Instruction {
    char *opcode;
    int mnemonic;
    char **operands;
    unsigned int opCount;
    word address;
} Instruction;

static const tableStruct condition[] = {{"eq", EQ}, 
                                        {"ne", NE}, 
                                        {"ge", GE},
                                        {"lt", LT}, 
                                        {"gt", GT}, 
                                        {"le", LE},
                                        {"al", AL}};

static const tableStruct shift[] = {{"lsl", LSL}, {"lsr", LSR}, {"asr", ASR}, {"ror", ROR}};

static const tableStruct opcode[] = {{"and", AND}, 
                                          {"eor", EOR}, 
                                          {"sub", SUB},     
                                          {"rsb", RSB},
                                          {"add", ADD}, 
                                          {"tst", TST}, 
                                          {"teq", TEQ},     
                                          {"cmp", CMP},
                                          {"orr", ORR}, 
                                          {"mov", MOV}, 
                                          {"andeq", ANDEQ}, 
                                          {"lsl", LSL_2},
                                          {"ldr", LDR}, 
                                          {"str", STR}, 
                                          {"mul", MUL},     
                                          {"mla", MLA},
                                          {"beq", BEQ}, 
                                          {"bne", BNE}, 
                                          {"bge", BGE},     
                                          {"blt", BLT},
                                          {"bgt", BGT}, 
                                          {"ble", BLE}, 
                                          {"b", B}};

int lookup(const tableStruct table[], const char *key, const int size);

unsigned int immediateVal(char *operand);

word assembleMultiply(SymbolTable *symbolTable, Instruction instruction);

word assembleBranch(SymbolTable *symbolTable, Instruction instruction);

word assembleDPI(SymbolTable *symbolTable, Instruction instruction);

word assembleSDTI(SymbolTable *symbolTable, Instruction instruction);

word tokenizeLine(SymbolTable *symbolTable, const char *line, word address);

void firstPass(FILE *assemblyFile, SymbolTable *table, File *lines);

#endif
