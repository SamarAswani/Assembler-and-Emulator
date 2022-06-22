#ifndef ASSEMBLER_FUNCTIONS
#define ASSEMBLER_FUNCTIONS

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "constants.h"
#include "loadArmLines.h"
#include "symbolTable.h"

#include <stdio.h>

typedef struct {
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

#endif