#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "assembler/symbolTable.h"

#include "assembler/constants.h"
#include "assembler/loadArmLines.h"
#include "assembler/symbolTable.h"
#include "assembler/assemblerFunctions.h"

int main(int argc, char **argv) {
  SymbolTable *symbolTable = createSymbolTable();
  Symbol definedSymbols[SYMBOLS] = {
      {strptr("add"), OPCODE, .value.assembleFunction = assembleDPI, NULL, NULL},
      {strptr("sub"), OPCODE, .value.assembleFunction = assembleDPI, NULL, NULL},
      {strptr("rsb"), OPCODE, .value.assembleFunction = assembleDPI, NULL, NULL},
      {strptr("and"), OPCODE, .value.assembleFunction = assembleDPI, NULL, NULL},
      {strptr("eor"), OPCODE, .value.assembleFunction = assembleDPI, NULL, NULL},
      {strptr("orr"), OPCODE, .value.assembleFunction = assembleDPI, NULL, NULL},
      {strptr("mov"), OPCODE, .value.assembleFunction = assembleDPI, NULL, NULL},
      {strptr("tst"), OPCODE, .value.assembleFunction = assembleDPI, NULL, NULL},
      {strptr("teq"), OPCODE, .value.assembleFunction = assembleDPI, NULL, NULL},
      {strptr("cmp"), OPCODE, .value.assembleFunction = assembleDPI, NULL, NULL},
      {strptr("mul"), OPCODE, .value.assembleFunction = assembleMultiply, NULL, NULL},
      {strptr("mla"), OPCODE, .value.assembleFunction = assembleMultiply, NULL, NULL},
      {strptr("ldr"), OPCODE, .value.assembleFunction = assembleSDTI, NULL, NULL},
      {strptr("str"), OPCODE, .value.assembleFunction = assembleSDTI, NULL, NULL},
      {strptr("beq"), OPCODE, .value.assembleFunction = assembleBranch, NULL, NULL},
      {strptr("bne"), OPCODE, .value.assembleFunction = assembleBranch, NULL, NULL},
      {strptr("bge"), OPCODE, .value.assembleFunction = assembleBranch, NULL, NULL},
      {strptr("blt"), OPCODE, .value.assembleFunction = assembleBranch, NULL, NULL},
      {strptr("bgt"), OPCODE, .value.assembleFunction = assembleBranch, NULL, NULL},
      {strptr("ble"), OPCODE, .value.assembleFunction = assembleBranch, NULL, NULL},
      {strptr("b"), OPCODE, .value.assembleFunction = assembleBranch, NULL, NULL},
      {strptr("lsl"), OPCODE, .value.assembleFunction = assembleDPI, NULL, NULL},
      {strptr("andeq"), OPCODE, .value.assembleFunction = assembleDPI, NULL, NULL}};
  for (int i = 0; i < SYMBOLS; i++) {
    add(symbolTable, &definedSymbols[i]);
  }
  
  ArmLines *armLines = initArmLines();

  FILE *armFile = fopen(argv[1], "r");
  firstPass(armFile, symbolTable, armLines);
  fclose(armFile);

  FILE *output = fopen(argv[2], "wb");
  secondPassLines(armLines, symbolTable, output);
  fclose(output);

  // freeSymbol(symbols);
  // freeSymbolTable(symbolTable);
  freeLines(armLines);

  return EXIT_SUCCESS;
}
