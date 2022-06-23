#ifndef ASSEMBLE_CONSTANTS_H
#define ASSEMBLE_CONSTANTS_H

#include <stdlib.h>

#define INITIAL_SIZE 8
#define HEX 16
#define MAX_TOKEN_COUNT 5
#define MAX (0xFF)
#define ALWAYS 0xE
#define ARM_OFFSET 8
#define DEFAULT_ARGS 3
#define SYMBOLS 23
#define WORD_TO_BYTE 4
#define ROTATE 2
#define ROTATION 8

#define START (ALWAYS << COND_SHIFT)

#define MULTIPLY 0x90

#define BRANCH 0x0A000000

#define SET_SHIFT (20)

#define SDT 0x4000000
#define SDTI_BOUND 0xFF
#define POST_IDX 3

#define SHIFT_REG 1 << 4
#define SHIFT_NUM 7
#define SHIFT_TYPE_ASS 5
#define MAX_LINE_SIZE (512)

#endif
