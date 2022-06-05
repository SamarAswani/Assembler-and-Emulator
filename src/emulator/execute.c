#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "execute.h"
#include "decode.h"

static void executeDPI(State *state, DataProcessingInstruction *decoded) {
}

static void executeMultiply(State *state, MultiplyInstruction *decoded) {
}

static void executeSDTI(State *state, SingleDataTransferInstruction *decoded) {
}

static void executeBranch(State *state, BranchInstruction *decoded) {
}

void execute(State *state, DecodedInstruction *decoded) {
    word instruction = state->decoded.instruction;
    if (instruction == 0) { return; }
    // check condition and return
    switch (state->decoded.type) {
        case DPI:
            executeDPI(state, decoded->dp);
            break;
        case MULT:
            executeMultiply(state, decoded->multiply);
            break;
        case SDTI:
            executeSDTI(state, decoded->sdt);
            break;
        case BR:
            executeBranch(state, decoded->branch);
            break;
    default:
        perror("Instruction not supported");
        exit(EXIT_FAILURE);
    }
}
