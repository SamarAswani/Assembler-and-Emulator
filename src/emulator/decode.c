#include <stdlib.h>
#include <stdio.h>
#include "decode.h"

static void decodeDPI(State *state, word instruction) {
    state->decoded.dp.cond = (instruction & COND_MASK) >> COND_SHIFT;
    state->decoded.dp.i = (instruction & DPI_I) >> DPI_I_SHIFT;
    state->decoded.dp.opcode = (instruction & DPI_OPCODE) >> DPI_OPCODE_SHIFT;
    state->decoded.dp.s = (instruction & CSPR_MASK) >> CSPR_SHIFT;
    state->decoded.dp.rn = (instruction & DPI_RN) >> DPI_RN_SHIFT;
    state->decoded.dp.rd = (instruction & DPI_RD) >> DPI_RD_SHIFT;
    state->decoded.dp.op2 = instruction & DPI_OP2;
}

static void decodeMultiply(State *state, word instruction) {
    state->decoded.multiply.cond = (instruction & COND_MASK) >> COND_SHIFT;
    state->decoded.multiply.a = (instruction & ACC_MASK) >> ACC_SHIFT;
    state->decoded.multiply.s = (instruction & CSPR_MASK) >> CSPR_SHIFT;
    state->decoded.multiply.rd = (instruction & RD_MASK) >> RD_SHIFT;
    state->decoded.multiply.rs = (instruction & RN_MASK) >> RN_SHIFT;
    state->decoded.multiply.rn = (instruction & RS_MASK) >> RS_SHIFT;
    state->decoded.multiply.rm = instruction & RM_MASK;
}

static void decodeSDTI(State *state, word instruction) {
    state->decoded.sdt.cond = (instruction & COND_MASK) >> COND_SHIFT;
    state->decoded.sdt.i = (instruction & SDTI_I_MASK) >> SDTI_I_SHIFT;
    state->decoded.sdt.p = (instruction & SDTI_P_MASK) >> SDTI_P_SHIFT;
    state->decoded.sdt.u = (instruction & SDTI_U_MASK) >> SDTI_U_SHIFT;
    state->decoded.sdt.l = (instruction & SDTI_L_MASK) >> SDTI_L_SHIFT;
    state->decoded.sdt.rn = (instruction & SDTI_RN_MASK) >> SDTI_RN_SHIFT;
    state->decoded.sdt.rd = (instruction & SDTI_RD_MASK) >> SDTI_RD_SHIFT;
    state->decoded.sdt.offset = instruction & SDTI_OFFSET_MASK;
}

static void decodeBranch(State *state, word instruction) {
    state->decoded.branch.cond = (instruction & COND_MASK) >> COND_SHIFT;
    state->decoded.branch.offset = instruction & OFFSET;
}

void decode(State *state) {
    word instruction = state->fetched;
    state->decoded.instruction = instruction;
    if (state->fetched == 0) { return; }

    if ((instruction & DPI_MASK) == IS_DPI) {
        state->decoded.type = DPI;
        decodeDPI(state, instruction);
    } 
    
    else if ((instruction & MULT_MASK) == IS_MULT) {
        state->decoded.type = MULT;
        decodeMultiply(state, instruction);
    } 
    
    else if ((instruction & SDT_MASK) == IS_SDT) {
        state->decoded.type = SDTI;
        decodeSDTI(state, instruction);
    } 
    
    else if ((instruction & BRANCH_MASK) == IS_BRANCH) {
        state->decoded.type = BR;
        decodeBranch(state, instruction);
    }

    state->decoded.isSet = true;
}
