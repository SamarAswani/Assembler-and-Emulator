#include "assemblerFunctions.h"
#include "constants.h"
#include "loadArmLines.h"
#include "symbolTable.h"
#include "../emulator/instruction.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int lookup(const tableStruct table[], const char *key, const int size) {
    for (int i = 0; i < size; i++) {
        if (strcmp(table[i].key, key) == 0) {
            return table[i].value;
        }
    }
    return -1;
}

unsigned int immediateVal(char *operand) {
    if (strlen(operand) >= 3) {
      if (operand[0] == '0' && operand[1] == 'x') {
        return (unsigned int) strtol(operand, NULL, HEX);
      }
    }
    if (operand[0] == '-') {
      ++operand;
    }
    return (unsigned int) atoi(operand);
}

word assembleMultiply(SymbolTable *symbolTable, Instruction instruction) {
    word rd = atoi(++instruction.operands[0]) << RD_SHIFT;
    word rm = atoi(++instruction.operands[1]);
    word rs = atoi(++instruction.operands[2]) << RS_SHIFT;
    word rn = 0;
    word acc = 0;

    if (instruction.mnemonic == MLA) {
      rn = atoi(++instruction.operands[3]) << RN_SHIFT;
      acc = 1 << ACC_SHIFT;
    }

    return MULT_START | acc | rd | rn | rs | MULTIPLY | rm;
}

word assembleBranch(SymbolTable *symbolTable, Instruction instruction) {
    word cond;
    if (instruction.mnemonic == B) {
      cond = BRANCH_START;
    } else {
      cond = lookup(condition, ++instruction.opcode, 7) << COND_SHIFT;
    }

    char *line = instruction.operands[0];

    word newAddress;
    if (line[0] == '#' || line[0] == '=') {
      newAddress = atoi(++line);
    } else {
      Symbol *symbol = get(symbolTable, line);
      if (symbol == NULL) {
        printf("Error: Symbol %s not found\n", line);
        exit(1);
      }
      newAddress = symbol->value.address;
    }

    word offset = ((newAddress - instruction.address - ARM_OFFSET) >> INSTRUCTION_ADDRESS_TO_MEM_ADDRESS) & BRANCH_OFFSET_MASK;
    return cond | BRANCH | offset;
}

word assembleDPI(Symbol *symbolTable, Instruction instruction) {
    word opcode = instruction.mnemonic;
    word rd = 0;
    char *imm;
    word s = 0;
    word rn = 0;
    char **op2;
    unsigned int args;
    word i;
    word operand2;

    if (opcode == ANDEQ) {
      return 0x00000000;
    } else if (opcode == LSL_2) {
      opcode = MOV;
      char *rdTemp = instruction.operands[0];
      rd = atoi(++instruction.operands[0]);
      imm = instruction.operands[0];
      char *operands[DEFAULT_ARGS] = {rdTemp, "lsl", instruction.operands[1]};
      op2 = operands;
      args = DEFAULT_ARGS;
    } else if (opcode == MOV) {
      rd = atoi(++instruction.operands[0]);
      imm = instruction.operands[1];
      op2 = instruction.operands + 1;
      args = instruction.opCount - 1;
    } else if (opcode == TST || opcode == CMP || opcode == TEQ) { 
      imm = instruction.operands[1];
      rn = atoi(++instruction.operands[0]);
      op2 = instruction.operands + 1;
      args = instruction.opCount - 1;
      s=1;
    } else {
      rd = atoi(++instruction.operands[0]);
      rn = atoi(++instruction.operands[1]);
      imm = instruction.operands[2];
      op2 = instruction.operands + 2;
      args = instruction.opCount - 2;
    }

    opcode = opcode << DPI_OPCODE_SHIFT;
    s = s << SET_SHIFT;
    rn = rn << DPI_RN_SHIFT;
    rd = rd << DPI_RD_SHIFT;

    if (imm[0] == '#' || imm[0] == '=') {
      i = 1 << DPI_I_SHIFT;
    } else {
      i = 0;
    }

    if (op2[0][0] == '#' || op2[0][0] == '=') {
      // immediate
    } else {
      // register
    }

    return DPI_START | i | opcode | s | rn | rd | operand2;
}

word assemble(SymbolTable *symbolTable, Instruction instructionIn) {
  Symbol *symbol = getSymbol(symbolTable, instructionIn.opcode);
  if (symbol == NULL) {
    return 0;
  }
  word lineReturn;
  if (symbol->type == OPCODE) {
    lineReturn = symbol->value.assembleFunction(symbolTable, instructionIn);
  } else {
    lineReturn = immediateVal(instructionIn.opcode + 1);
  }
  return lineReturn;
}

word tokenizeLine(SymbolTable *symbolTable, const char *line, word address) {
    char *other = NULL;
    char *lineTemp = strptr(line);

    char *tokens[MAX_TOKEN_COUNT];
    int count = 0;
    char *token = strtok_r(lineTemp, " ,", &other);
    while (token != NULL) {
        if (other[0] == ' ') {
            ++other;
            continue;
        }
    tokens[count++] = token;
    if (other[0] == '[') { 
      token = strtok_r(other, "]", &other);
    } else {
      token = strtok_r(NULL, " ,", &other);
    }
  }
  
  Instruction instruction = {tokens[0], lookup(opcode, SYMBOLS, tokens[0]), tokens + 1, count-1, address};
  word lineReturn = assemble(symbolTable, instruction);
  free(lineTemp);
  return lineReturn;
} 



void secondPassLines(File *file, SymbolTable *symbolTable, FILE *out) {
  for (int line = 0; line < file->count; line++) {
    word lineWrite = parseLine(symbolTable, file->lines[line], line * WORD_TO_BYTE);
    fwrite(&lineWrite, sizeof(word), 1, out);
  }
}



