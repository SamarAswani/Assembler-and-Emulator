#ifndef ASSEMBLE_CONSTANTS_H
#define ASSEMBLE_CONSTANTS_H

#include <stdlib.h>

#define INITIAL_SIZE 8
#define HEX 16
#define MAX_TOKEN_COUNT 5
#define ALWAYS 0xE
#define ARM_OFFSET 8
#define DEFAULT_ARGS 3
#define SYMBOLS 23
#define WORD_TO_BYTE 4

#define MULTIPLY 0x90
#define MULT_START (ALWAYS << COND_SHIFT)

#define BRANCH_START (ALWAYS << COND_SHIFT)
#define BRANCH 0x0A000000

#define SET_SHIFT (20)
#define DPI_START (ALWAYS << COND_SHIFT)

#endif