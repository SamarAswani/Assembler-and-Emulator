#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "execute.h"
#include "decode.h"
#include "fetch.h"
#include "instruction.h"

void updateCPSR(State *state, word res, unsigned int carry) {
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
    // unsigned int c = (state->registers[CPSR] & (1 << C_SHIFT)) >> C_SHIFT;
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


// rotateRight Implementation 2:
static word rotateRight(word imm, unsigned int rotate) {
    unsigned int lsbs = imm & ((1 << rotate) - 1);
    return (imm >> rotate) | (lsbs << (WORD_IN_BITS - rotate));
    // return (val >> rotate) | (val << (WORD_IN_BITS-rotate));

}

static unsigned int rotateRightCarry(word imm, unsigned int rotate) {
    if (rotate == 0) {
        return 0;
    }
    return (imm >> (rotate - 1)) & CARRY_MASK;
}


static void store(State *state, word rd, word rn) {
    if(rn > MEM_SIZE) {
        printf("Error: Out of bounds memory access at address 0x%08x\n", rn);
        return;
    }
    for (int i = 0; i < WORD_IN_BYTES; i ++) {
        state->memory[rn + i] = state->registers[rd] >> BYTE * i;
    }
}

static void load(State *state, word rd, word rn) {
    if (rn > MEM_SIZE) {
        printf("Error: Out of bounds memory access at address 0x%08x\n", rn);
        return;
    }
    state->registers[rd] = getWord(state, rn);
}


static unsigned int leftCarry (word val, unsigned int shiftVal ) {
    if (shiftVal == 0) {
        return 0;
  }

  return (val << (shiftVal - 1)) >> (WORD_SIZE - 1);
}

// static unsigned int rightCarry(word val, unsigned int shiftVal) {
//     if (shiftVal == 0) {
//         return 0;
//     }
//     return (val >> (shiftVal -1)) & LSB;
// }

static word arithmeticShiftRight(word val, unsigned int shiftVal) {
    word msb = val & MSB;
    word msbs = msb;
    for (int i = 0; i < shiftVal; i++ ) {
        msbs = msbs >> 1;
        msbs = msbs + msb;
    }
    word res = (msbs | (val >> shiftVal));
    return res;
}

static OperationInstruction *barrelShifter(State *state, word val, unsigned int shift) {
   unsigned int shiftVal = shift >> DPI_SHIFT_CONSTANT;
   Shift type = (shift & SHIFT_MASK) >> SHIFT_TYPE;
   OperationInstruction* resShift = malloc(sizeof(OperationInstruction));
   if (resShift == NULL) {
    perror("null pointer");
    exit(MEM_SIZE);
   }
   word res;
   unsigned int carry;

   switch (type) {
    case ASR:
        carry = rotateRightCarry(val, shiftVal);
        res = arithmeticShiftRight(val, shiftVal);  
        break;

    case LSR:
        carry = rotateRightCarry(val, shiftVal);
        res = val >> shiftVal;
        break;

    case LSL:
        carry = leftCarry(val, shiftVal);
        res = val << shiftVal;
        break;
    
    case ROR:
        // carry = rightCarry(val, shiftVal);
        carry = rotateRightCarry(val, shiftVal);
        // res = rotateRight2(val, shiftVal);
        res = rotateRight(val, shiftVal);
        break;

    default:
        perror("case error");
        exit(EXIT_FAILURE);
   }

   resShift -> result = res, resShift -> carry = carry;
   return resShift;

}

OperationInstruction *registerOperand (State *state, unsigned int operand) {
    word val = state->registers[operand & OFFSET_RM_MASK];
    return barrelShifter(state, val, operand >> SHIFT);

}

OperationInstruction *immediateOperand (State *state, unsigned int operand) {
    word imm = (operand & LEAST_BYTE);
    unsigned int rotate = ((operand & OFFSET_ROTATE_MASK) >> ROTATE_SHIFT) * ROTATION_MULT;
    OperationInstruction* resShift = malloc(sizeof(OperationInstruction));
    if (resShift == NULL) {
        perror("null pointer");
        exit(MEM_SIZE);
    }

    resShift->result = rotateRight(imm, rotate);
    resShift->carry = rotateRightCarry(imm, rotate);
    return resShift;
}

static word arithmeticCarryOut(word operand1, word operand2, bool add) {
    if (add) {
        if (operand1 <= UINT32_MAX - operand2) {
            return 0;
        }
        return 1;
    }

    if (operand1 < operand2) {
        return 0;
    }

    return 1;
}

static void executeDPI(State *state) {
    OperationInstruction *opShifted;
    if (state->decoded.i.dp.i) {
        opShifted = immediateOperand(state, state->decoded.i.dp.op2);
    }
    else {
        opShifted = registerOperand(state, state->decoded.i.dp.op2);
    }
    word operand1 = state->decoded.i.dp.op1;
    word operand2 = opShifted->result;
    unsigned int carry = opShifted->carry;
    unsigned int rd = state->decoded.i.dp.rd;

    word res;

    switch (state->decoded.i.dp.opcode) {
        case AND:
            res = operand1 & operand2;
            state->registers[rd] = res;
            break; 
        case EOR:
            res = operand1 ^ operand2;
            state->registers[rd] = res;
            break;
        case SUB:
            res = operand1 - operand2;
            carry = arithmeticCarryOut(operand1, operand2, false);
            state->registers[rd] = res;
            break;
        case RSB:
            res = operand2 - operand1;
            carry = arithmeticCarryOut(operand2, operand1, false);
            state->registers[rd] = res;
            break;
        case ADD:
            res = operand1 + operand2;
            carry = arithmeticCarryOut(operand1, operand2, true);
            state->registers[rd] = res;
            break;
        case TST:
            res = operand1 & operand2;
            break;
        case TEQ:
            res = operand1 ^ operand2;
            break;
        case CMP:
            res = operand1 - operand2;
            carry = arithmeticCarryOut(operand1, operand2, false);
            break;    
        case ORR:
            res = operand1 | operand2;
            state->registers[rd] = res;
            break;
        case MOV:
            res = operand2;
            state->registers[rd] = res;
            break;
        default:
            perror("case error");    
            exit(EXIT_FAILURE);
    }
    if (state->decoded.i.dp.s) {
        updateCPSR(state, res, carry);
    }
    free(opShifted);
    // free(state->decoded);
     
}

static void executeMultiply(State *state) {
    int res = state->registers[state->decoded.i.multiply.s] * state->registers[state->decoded.i.multiply.rm];
    if (state->decoded.i.multiply.a) {
        res += state->registers[state->decoded.i.multiply.rn];
    }
    if (state->decoded.i.multiply.s) {
        unsigned int c = (state->registers[CPSR] & (1 << C_SHIFT)) >> C_SHIFT;
        updateCPSR(state, res, c);
    }
    state->registers[state->decoded.i.multiply.rd] = res;
}

static void executeSDTI(State *state) {
    OperationInstruction *shift = state->decoded.i.sdt.i 
    ? registerOperand(state, state->decoded.i.sdt.offset) 
    : immediateOperand(state, state->decoded.i.sdt.offset);
    word *rn = state->registers + state->decoded.i.sdt.rn;
    word offset = shift->result;
    if (!state->decoded.i.sdt.u) {
        offset = -offset;
    }
    if (state->decoded.i.sdt.p) {
        if(state->decoded.i.sdt.l) {
            load(state, state->decoded.i.sdt.rd, *rn + offset);
        }
        else {
            store(state, state->decoded.i.sdt.rd, *rn + offset);
        }
    }
    else {
        if(state->decoded.i.sdt.l) {
            load(state, state->decoded.i.sdt.rd, *rn);
        }
        else {
            store(state, state->decoded.i.sdt.rd, *rn);
        }
        *rn += offset;
    }
    free(shift);
}

static void flushPipeline(State *state) {
    state->fetched = 0;
    state->decoded.instruction = 0;
    state->decoded.isSet = false;
}

static void executeBranch(State *state) {
    flushPipeline(state);
    int offset = state->decoded.i.branch.offset;
    int signBit = offset & BRANCH_SIGN_BIT;
    int pcOffset = ((offset << INSTRUCTION_ADDRESS_TO_MEM_ADDRESS) | (signBit ? NEGATIVE_SIGN_EXTEND : POSITIVE_SIGN_EXTEND));
    state->registers[PC] += pcOffset;
}

void execute(State *state) {
    word instruction = state->decoded.instruction;
    if (!state->decoded.isSet || !conditions(state, instruction)) { return; }
    printf("%x\n", instruction);
    switch (state->decoded.type) {
        case DPI:
            printf("dpi\n");
            executeDPI(state);
            break;
        case MULT:
            printf("mult\n");
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
