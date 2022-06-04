#ifndef DECODE
#define DECODE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../definitions.h"
#include "instruction.h"

extern void decode(State *state, DecodedInstruction *decoded);

#endif
