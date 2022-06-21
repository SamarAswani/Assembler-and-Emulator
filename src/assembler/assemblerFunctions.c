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

static word *SDTIparser(const char* string) {
    word *addressRegExp = malloc(sizeof(word*) *4);
    if (addressRegExp == NULL) {
        printf("Error: NULL pointer.");
        exit(EXIT_FAILURE);
    }
//    separator
    char *sep = ", ";
//    remove initial bracket
    char *stringTemp = strptr(string + 1);
//    rn
    char *token = strtok(stringTemp, sep);
    if (token[0] == 'r') {
//        for constant or register
        (++token);
    }
    addressRegExp[0] = immediateVal(token);
    char* secondToken = strtok(NULL, sep);
//    checking of existence of expression
    if (secondToken != NULL) {
        addressRegExp[3] = token[0] == 'r' ? 1 : 0
        addressRegExp[2] = (++token)[0] == '-' ? 0 : 1
        addressRegExp[1] = immediateVal(secondToken);
    }
    free(stringTemp);
    return addressRegExp;
}

static STDIAddressType getSTDIAddressType(char **operands, unsigned int opCount) {
    if (opCount == POST_IDX) {
        return POST_IDX_EXP;
    }
    if (strstr(operands[1], ",")) {
        return PRE_IDX_EXP;
    }
    if (strstr(operands[1], "r")) {
        return PRE_IDX;
    }
    return NUMERIC_CONST;
}
word assembleSDTI(SymbolTable *symbolTable, Instruction instruction) {
//    word cond = lookup(symbolTable, ++instruction.opcode, 2) << COND_SHIFT;
    word i = 0;

    STDIAddressType addressType = getSTDIAddressType(instruction.operands, instruction.opCount );
    if (addressType == POST_IDX_EXP) {
        word p = 1 << SDTI_P_SHIFT;
    } else {
        word p = 0;
    }

    word u = 1 << SDTI_U_SHIFT;

    if (instruction.mnemonic == LDR) {
       word l = 1 << SDTI_L_SHIFT;
    } else {
        word l = 0;
    }

    word* addresses = SDTIparser(instruction.operands[1])
    word rn = addresses[0] << SDTI_RN_SHIFT;

    word rd = atoi(++instruction.operands[0]) << SDTI_RD_SHIFT;

    word offset;

    switch (addressType) {
        case POST_IDX_EXP:
            offset = atoi((++instruction.operands[2]));
//            could add optional post immediate offset
            break;
        case PRE_IDX:
            offset = 0;
            break;
        case PRE_IDX_EXP:
            u = addresses[2] << SDTI_U_SHIFT;
            offset = addresses[1];
//            could add optional
        case NUMERIC_CONST:
            if (immediateVal(input.fields[1] + 1) <= SDTI_BOUND) {
                free(addresses);
                instruction.opcode = "mov";
                instruction.mnemonic = MOV;
                return assembleDPI(symbolTable, instruction);
            } else {
                // offset
                offset = lookup(symbolTable, input.fields[1], 2)->value.address - ARM_OFFSET;
                offset -= =instruction.currentAddress;
                // Base register Rn
                rn = PC << SDTI_RN_SHIFT;
            }
            break;
        default:
            printf("Error: Unexpected SDTI addressing method");
            exit(EXIT_FAILURE);
    }

    free(addresses);

    return SDTI_START | SDTI | i | p | u | l | rn | rd | offset;
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