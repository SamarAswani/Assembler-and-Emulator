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

static void store(State *state, word rd, word rn) {
    if(rn > MEM_SIZE) {
        perror("Tried to access memory out of bounds.");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < WORD_IN_BYTES; i++) {
        state->memory[rn + i] = state->registers[rd] >> BYTE * i;
    }
}

static void executeDPI(State *state) {
}

static void executeMultiply(State *state) {
    int res = state->registers[state->decoded.multiply.s] * state->registers[state->decoded.multiply.rm];
    if (state->decoded.multiply.a) {
        res += state->registers[state->decoded.multiply.rn];
    }
    if (state->decoded.multiply.s) {
        unsigned int c = (state->registers[CPSR] & (1 << C_SHIFT)) >> C_SHIFT;
        updateCSPR(state, res, c);
    }
    state->registers[state->decoded.multiply.rd] = res;
}

static void executeSDTI(State *state) {
    ShiftInstruction *shift;
    if(state->decoded.sdt.i) {
        word rmValue = state->registers[state->decoded.sdt.offset & OFFSET_RM_MASK];
        unsigned int shiftBy = state->decoded.sdt.offset >> SHIFT;
        ShiftInstruction *shift = shifter(state, rmValue, shiftBy);
    }
    else {
        word imm = state->decoded.sdt.offset & OFFSET_IMMEDIATE_MASK;
        unsigned int rotate = ((state->decoded.sdt.offset & OFFSET_ROTATE_MASK) >> ROTATE_SHIFT) * 2;
        ShiftInstruction *shift = malloc(sizeof(*shift));
        shift->result = rotateRight(imm, rotate);
        shift->carry = rotateRightCarry(imm, rotate);
    }

    word *rn = state->registers + state->decoded.sdt.rn;
    if (state->decoded.sdt.u) {
        *rn += shift->result;
    }
    else {
        *rn -= shift->result;
    }
    if (state->decoded.sdt.p) {
        //store and load instructions to be implemented
    }
    free(shift);
}

static void executeBranch(State *state) {
}
//HEY QUEEEEEENNNNNNNNNN X
// <3 

void execute(State *state) {
    word instruction = state->decoded.instruction;
    if (instruction == 0 || !conditions(state, instruction)) { return; }
    switch (state->decoded.type) {
        case DPI:
            executeDPI(state);
            break;
        case MULT:
            executeMultiply(state);
            break;
        case SDTI:
            executeSDTI(state);
            break;
        case BR:
            executeBranch(state);
            break;
    default:
        perror("Instruction not supported");
        exit(EXIT_FAILURE);
    }
}
