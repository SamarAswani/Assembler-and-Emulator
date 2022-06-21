#ifndef ASSEMBLER_FUNCTIONS
#define ASSEMBLER_FUNCTIONS

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "constants.h"
#include "loadArmLines.h"
#include "symbolTable.h"

#include <stdio.h>

int lookup(const tableStruct table[], const char *key, const int size);

unsigned int immediateVal(char *operand);

word assembleMultiply(SymbolTable *symbolTable, Instruction instruction)

word assembleBranch(SymbolTable *symbolTable, Instruction instruction);

word tokenizeLine(SymbolTable *symbolTable, const char *line, word address);

typedef enum { NUMERIC_CONST, PRE_IDX, PRE_IDX_EXP, POST_IDX_EXP } SDTIAddressType;

typedef struct {
  char *key;
  word value;
} tableStruct;


static const tableStruct condTable[] = {{"eq", EQ}, 
                                        {"ne", NE}, 
                                        {"ge", GE},
                                        {"lt", LT}, 
                                        {"gt", GT}, 
                                        {"le", LE},
                                        {"al", AL}};

static const tableStruct shiftTable[] = {{"lsl", LSL}, {"lsr", LSR}, {"asr", ASR}, {"ror", ROR}};

static const tableStruct opcodeTable[] = {{"and", AND}, 
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

#endif