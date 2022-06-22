#ifndef ASSEMBLE_CONSTANTS_H
#define ASSEMBLE_CONSTANTS_H

#include <stdlib.h>

#define INITIAL_SIZE 8
#define HEX 16
#define MAX_TOKEN_COUNT 5
#define ALWAYS 0xE
#define ARM_OFFSET 8

#define START (ALWAYS << COND_SHIFT)

#define MULT 0x90

#define BRANCH 0x0A000000

#define SDTI 0x4000000
#define SDTI_BOUND 0xFF
#define POST_IDX 3
#endif