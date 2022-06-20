#include "assemblerFunctions.h"
#include "constants.h"
#include "loadArmLines.h"
#include "symbolTable.h"
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

word getLine(SymbolTable *symbolTable, const char *line, word address) {
    char *other = NULL;
    char *lineTemp = strptr(line);

    char *fields[MAX_FIELD_COUNT];
    int count = 0;
    char *token = strtok_r(lineTemp, " ,", &other);
    while (token != NULL) {
        if (other[0] == ' ') {
            ++other;
            continue;
        }
    fields[count++] = token;
    if (other[0] == '[') { 
      token = strtok_r(other, "]", &other);
    } else {
      token = strtok_r(NULL, " ,", &other);
    }
  }
}   