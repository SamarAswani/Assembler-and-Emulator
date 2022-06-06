#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "execute.h"
#include "decode.h"

void updateCSPR(State *state, word res, unsigned int carry) {
    word n = res & (1 << N_SHIFT);
    word z;
    word c;
    word v = state->registers[CPSR] & (1 << V_SHIFT);
    if(res) {
        z = 0;
    }
    else {
        z = (1 << Z_SHIFT);
    }

    if (carry) {
        c = (1 << C_SHIFT);
    }
    else {
        c = 0;
    }
    state->registers[CPSR] = n | z | c | v;
}

static bool conditions(State *state, word instruction) {
    unsigned int n = (state->registers[CPSR] & (1 << N_SHIFT)) >> N_SHIFT;
    unsigned int z = (state->registers[CPSR] & (1 << Z_SHIFT)) >> Z_SHIFT;
    unsigned int c = (state->registers[CPSR] & (1 << C_SHIFT)) >> C_SHIFT;
    unsigned int v = (state->registers[CPSR] & (1 << V_SHIFT)) >> V_SHIFT;
    Cond cond = instruction >> COND_SHIFT;
    switch (cond) {
        case EQ:
            return z;
        case NE:
            return !z;
        case GE:
            return n == v;
        case LT:
            return n != v;
        case GT:
            return !z && (n == v);
        case LE:
            return z || (n != v);
        case AL:
            return true;
        default:
            perror("Instruction not supported");
            exit(EXIT_FAILURE);
            return false;
    }
}

static word rotateRight(word imm, unsigned int rotate) {
  return (imm >> rotate) | (imm << (WORD_IN_BITS-rotate));
}

static unsigned int rotateRightCarry(word imm, unsigned int rotate) {
  if (rotate == 0) {
    return 0;
  }
  return (imm >> (rotate - 1)) & CARRY_MASK;
}

static ShiftInstruction *shifter(State *state, word val, unsigned int shift) {
}

static void executeDPI(State *state, DataProcessingInstruction *decoded) {
}

static void executeMultiply(State *state, MultiplyInstruction *decoded) {
    int res = state->registers[decoded->s] * state->registers[decoded->rm];
    if (decoded->a) {
        res += state->registers[decoded->rn];
    }
    if (decoded->s) {
        unsigned int c = (state->registers[CPSR] & (1 << C_SHIFT)) >> C_SHIFT;
        updateCSPR(state, res, c);
    }
    state->registers[decoded->rd] = res;
    free(decoded);
}

static void executeSDTI(State *state, SingleDataTransferInstruction *decoded) {
    ShiftInstruction *shift;
    if(decoded->i) {
        word rmValue = state->registers[decoded->offset & OFFSET_RM_MASK];
        unsigned int shiftBy = decoded->offset >> SHIFT;
        ShiftInstruction *shift = shifter(state, rmValue, shiftBy);
    }
    else {
        word imm = decoded->offset & OFFSET_IMMEDIATE_MASK;
        unsigned int rotate = ((decoded->offset & OFFSET_ROTATE_MASK) >> ROTATE_SHIFT) * 2;
        ShiftInstruction *shift = malloc(sizeof(*shift));
        shift->result = rotateRight(imm, rotate);
        shift->carry = rotateRightCarry(imm, rotate);
    }

    word *rn = state->registers + decoded->rn;
    if (decoded->u) {
        *rn += shift->result;
    }
    else {
        *rn -= shift->result;
    }
    if (decoded->p) {
        //store and load instructions to be implemented
    }
    free(shift);
    free(decoded);
}

static void executeBranch(State *state, BranchInstruction *decoded) {
}

void execute(State *state, DecodedInstruction *decoded) {
    word instruction = state->decoded.instruction;
    if (instruction == 0 || !conditions(state, instruction)) { return; }
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
