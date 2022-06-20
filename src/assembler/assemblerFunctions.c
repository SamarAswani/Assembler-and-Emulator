#include "assemblerFunctions.h"
#include "constants.h"
#include "loadArmLines.h"
#include "symbolTable.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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