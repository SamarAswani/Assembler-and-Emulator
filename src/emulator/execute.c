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


//rotateRight Implementation 2:
static word rotateRight2(word val, unsigned int rotate) {
    unsigned int lsbs = val & ((1 << rotate) - 1);
    return (val >> rotate) | (lsbs << (WORD_SIZE - rotate));
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


static void store(State *state, word rd, word rn) {
    if(rn > MEM_SIZE) {
        perror("Tried to access memory out of bounds.");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < WORD_IN_BYTES; i ++) {
        state->memory[rn + i] = state->registers[rd] >> (i * BYTE);
    }
}

static void load(State *state, word rd, word rn) {
    state->registers[rd] = getWord(state, rn);
}


static unsigned int leftCarry (word val, unsigned int shiftVal ) {
    if (shiftVal == 0) {
        return 0;
  }

  return (val << (shiftVal - 1)) >> (WORD_SIZE - 1);
}

static unsigned int rightCarry(word val, unsigned int shiftVal) {
    if (shiftVal == 0) {
        return 0;
    }
    return (val >> (shiftVal -1)) & LSB;
}

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



//pass in only shift not Rm
static OperationInstruction *barrelShifter(State *state, word val, unsigned int shift) {
   unsigned int shiftVal = shift >> DPI_SHIFT_CONSTANT;
   Shift type = (shift & SHIFT_MASK) >> SHIFT_TYPE;
   OperationInstruction* resShift = malloc(sizeof(OperationInstruction));
   if (resShift == NULL) {
    perror("null pointer");
    errorExit(MEM_SIZE);
   }
   word res;
   unsigned int carry;

   switch (type) {
    case ASR:
        carry = rightCarry(val, shiftVal);
        res = arithmeticShiftRight(val, shiftVal);  
        break;

    case LSR:
        carry = rightCarry(val, shiftVal);
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
        errorExit("case error");
   }

   resShift -> result = res, resShift -> carry = carry;
   return resShift;

}

OperationInstruction *registerOperand (State *state, unsigned int operand) {
    unsigned int RM = (operand & RM_MASK);
    word val = state -> registers[RM];
    unsigned int shift = operand >> SHIFT;
    return barrelShifter (state, val, shift);

}

OperationInstruction *immediateOperand (State *state, unsigned int operand) {
    word imm = (operand & LEAST_BYTE);
    unsigned int rotate = (operand >> ROTATE_SHIFT) * ROTATION_MULT;
    OperationInstruction* resShift = malloc(sizeof(OperationInstruction));
    if (resShift == NULL) {
        perror("null pointer");
        errorExit(MEM_SIZE);
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
    if (state->decoded.dp.i) {
        opShifted = immediateOperand(state, state->decoded.dp.op2);
    }
    else {
        opShifted = registerOperand(state, state->decoded.dp.op2);
    }
    word operand1 = state->decoded.dp.rn;
    word operand2 = opShifted->result;
    unsigned int carry = opShifted->carry;
    unsigned int rd = state->decoded.dp.rd;

    word res;

    switch (state->decoded.dp.opcode) {
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
            errorExit("case error");    
    }
    if (state->decoded.dp.s) {
        setCPSR(state, res, carry);
    }
    free(opShifted);
    // free(state->decoded);
     
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

static void executeSDTI(State *state) {
    OperationInstruction *shift;
    if(state->decoded.sdt.i) {
        word rmValue = state->registers[state->decoded.sdt.offset & OFFSET_RM_MASK];
        unsigned int shiftBy = state->decoded.sdt.offset >> SHIFT;
        OperationInstruction *shift = shifter(state, rmValue, shiftBy);
    }
    else {
        word imm = state->decoded.sdt.offset & OFFSET_IMMEDIATE_MASK;
        unsigned int rotate = ((state->decoded.sdt.offset & OFFSET_ROTATE_MASK) >> ROTATE_SHIFT) * 2;
        OperationInstruction *shift = malloc(sizeof(*shift));
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
