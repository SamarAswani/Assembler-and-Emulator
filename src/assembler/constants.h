#ifndef ASSEMBLE_CONSTANTS_H
#define ASSEMBLE_CONSTANTS_H

#include <stdlib.h>

#define INITIAL_SIZE 8
#define HEX 16
#define MAX_TOKEN_COUNT 5
#define ALWAYS 0xE
#define ARM_OFFSET 8
#define SYMBOLS 23
#define WORD_TO_BYTE 4

#define MULT 0x90
#define MULT_START (ALWAYS << COND_SHIFT)

#define BRANCH_START (ALWAYS << COND_SHIFT)
#define BRANCH 0x0A000000

#endif