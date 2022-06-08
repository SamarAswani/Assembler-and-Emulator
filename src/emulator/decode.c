#include <stdlib.h>
#include <stdio.h>
#include "decode.h"

static void decodeDPI(State *state, word instruction) {
    state->decoded.i.dp.cond = (instruction & COND_MASK) >> COND_SHIFT;
    state->decoded.i.dp.i = (instruction & DPI_I) >> DPI_I_SHIFT;
    state->decoded.i.dp.opcode = (instruction & DPI_OPCODE) >> DPI_OPCODE_SHIFT;
    state->decoded.i.dp.s = (instruction & CSPR_MASK) >> CSPR_SHIFT;
    state->decoded.i.dp.rn = (instruction & DPI_RN) >> DPI_RN_SHIFT;
    state->decoded.i.dp.rd = (instruction & DPI_RD) >> DPI_RD_SHIFT;
    state->decoded.i.dp.op2 = instruction & DPI_OP2;
}

static void decodeMultiply(State *state, word instruction) {
    state->decoded.i.multiply.cond = (instruction & COND_MASK) >> COND_SHIFT;
    state->decoded.i.multiply.a = (instruction & ACC_MASK) >> ACC_SHIFT;
    state->decoded.i.multiply.s = (instruction & CSPR_MASK) >> CSPR_SHIFT;
    state->decoded.i.multiply.rd = (instruction & RD_MASK) >> RD_SHIFT;
    state->decoded.i.multiply.rs = (instruction & RN_MASK) >> RN_SHIFT;
    state->decoded.i.multiply.rn = (instruction & RS_MASK) >> RS_SHIFT;
    state->decoded.i.multiply.rm = instruction & RM_MASK;
}

static void decodeSDTI(State *state, word instruction) {
    state->decoded.i.sdt.cond = (instruction & COND_MASK) >> COND_SHIFT;
    state->decoded.i.sdt.i = (instruction & SDTI_I_MASK) >> SDTI_I_SHIFT;
    state->decoded.i.sdt.p = (instruction & SDTI_P_MASK) >> SDTI_P_SHIFT;
    state->decoded.i.sdt.u = (instruction & SDTI_U_MASK) >> SDTI_U_SHIFT;
    state->decoded.i.sdt.l = (instruction & SDTI_L_MASK) >> SDTI_L_SHIFT;
    state->decoded.i.sdt.rn = (instruction & SDTI_RN_MASK) >> SDTI_RN_SHIFT;
    state->decoded.i.sdt.rd = (instruction & SDTI_RD_MASK) >> SDTI_RD_SHIFT;
    state->decoded.i.sdt.offset = instruction & SDTI_OFFSET_MASK;
}

static void decodeBranch(State *state, word instruction) {
    state->decoded.i.branch.cond = (instruction & COND_MASK) >> COND_SHIFT;
    state->decoded.i.branch.offset = instruction & OFFSET;
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
