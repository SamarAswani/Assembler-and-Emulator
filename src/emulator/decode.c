#include <stdlib.h>
#include <stdio.h>
#include "decode.h"
#include "instruction.h"

static DataProcessingInstruction *decodeDPI(State state, word instruction) {
    DataProcessingInstruction *decoded = malloc(sizeof(*decoded));
    decoded->cond = (instruction & COND_MASK) >> COND_SHIFT;
    decoded->i = (instruction & DPI_I) >> DPI_I_SHIFT;
    decoded->opcode = (instruction & DPI_OPCODE) >> DPI_OPCODE_SHIFT;
    decoded->s = (instruction & CSPR_MASK) >> CSPR_SHIFT;
    decoded->rn = (instruction & DPI_RN) >> DPI_RN_SHIFT;
    decoded->rd = (instruction & DPI_RD) >> DPI_RD_SHIFT;
    decoded->op2 = instruction & DPI_OP2;
    return decoded;
}

static MultiplyInstruction *decodeMultiply(State state, word instruction) {
    MultiplyInstruction *decoded = malloc(sizeof(*decoded));
    decoded->cond = (instruction & COND_MASK) >> COND_SHIFT;
    decoded->a = (instruction & ACC_MASK) >> ACC_SHIFT;
    decoded->s = (instruction & CSPR_MASK) >> CSPR_SHIFT;
    decoded->rd = (instruction & RD_MASK) >> RD_SHIFT;
    decoded->rs = (instruction & RN_MASK) >> RN_SHIFT;
    decoded->rn = (instruction & RS_MASK) >> RS_SHIFT;
    decoded->rm = instruction & RM_MASK;
    return decoded;
}

static SingleDataTransferInstruction *decodeSDTI(State state, word instruction) {
    SingleDataTransferInstruction *decoded = malloc(sizeof(*decoded));
    decoded->cond = (instruction & COND_MASK) >> COND_SHIFT;
    decoded->i = (instruction & SDTI_I_MASK) >> SDTI_I_SHIFT;
    decoded->p = (instruction & SDTI_P_MASK) >> SDTI_P_SHIFT;
    decoded->u = (instruction & SDTI_U_MASK) >> SDTI_U_SHIFT;
    decoded->l = (instruction & SDTI_L_MASK) >> SDTI_L_SHIFT;
    decoded->rn = (instruction & SDTI_RN_MASK) >> SDTI_RN_SHIFT;
    decoded->rd = (instruction & SDTI_RD_MASK) >> SDTI_RD_SHIFT;
    decoded->offset = instruction & SDTI_OFFSET_MASK;
    return decoded;
}

static BranchInstruction *decodeBranch(State state, word instruction) {
    BranchInstruction *decoded = malloc(sizeof(*decoded));
    decoded->cond = (instruction & COND_MASK) >> COND_SHIFT;
    decoded->offset = instruction & OFFSET;
    return decoded;
}

void decode(State state, DecodedInstruction *decoded) {
    word instruction = state.fetched;
    state.decoded.instruction = instruction;
    if (state.fetched == 0) { return; }

    if ((instruction & DPI_MASK) == IS_DPI) {
        state.decoded.type = DPI;
        state.decoded.dp = decodeDPI(state, instruction);
    } 
    
    else if ((instruction & MULT_MASK) == IS_MULT) {
        state.decoded.type = MULT;
        state.decoded.multiply = decodeMultiply(state, instruction);
    } 
    
    else if ((instruction & SDT_MASK) == IS_SDT) {
        state.decoded.type = SDTI;
        state.decoded.sdt = decodeSDTI(state, instruction);
    } 
    
    else if ((instruction & BRANCH_MASK) == IS_BRANCH) {
        state.decoded.type = BR;
        state.decoded.branch = decodeBranch(state, instruction);
    }
}