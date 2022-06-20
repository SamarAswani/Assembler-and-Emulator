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

    return MULT_START | acc | rd | rn | rs | MULT | rm;
}

word assembleBranch(SymbolTable *symbolTable, Instruction instruction) {
    word cond;
    if (instruction.mnemonic == B) {
      cond = BRANCH_START;
    } else {
      cond = lookup(condTable, ++instruction.opcode, 7) << COND_SHIFT;
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
}   