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
  Symbol **symbols = createLabelSymbol(SYMBOLS, sizeof(*symbols));
  Symbol definedSymbols[SYMBOLS] = {
      {strptr("add"), OPCODE, 0, .value.assembleFunction = assembleDPI},
      {strptr("sub"), OPCODE, 0, .value.assembleFunction = assembleDPI},
      {strptr("rsb"), OPCODE, 0, .value.assembleFunction = assembleDPI},
      {strptr("and"), OPCODE, 0, .value.assembleFunction = assembleDPI},
      {strptr("eor"), OPCODE, 0, .value.assembleFunction = assembleDPI},
      {strptr("orr"), OPCODE, 0, .value.assembleFunction = assembleDPI},
      {strptr("mov"), OPCODE, 0, .value.assembleFunction = assembleDPI},
      {strptr("tst"), OPCODE, 0, .value.assembleFunction = assembleDPI},
      {strptr("teq"), OPCODE, 0, .value.assembleFunction = assembleDPI},
      {strptr("cmp"), OPCODE, 0, .value.assembleFunction = assembleDPI},
      {strptr("mul"), OPCODE, 0, .value.assembleFunction = assembleMultiply},
      {strptr("mla"), OPCODE, 0, .value.assembleFunction = assembleMultiply},
      {strptr("ldr"), OPCODE, 0, .value.assembleFunction = assembleSDTI},
      {strptr("str"), OPCODE, 0, .value.assembleFunction = assembleSDTI},
      {strptr("beq"), OPCODE, 0, .value.assembleFunction = assembleBranch},
      {strptr("bne"), OPCODE, 0, .value.assembleFunction = assembleBranch},
      {strptr("bge"), OPCODE, 0, .value.assembleFunction = assembleBranch},
      {strptr("blt"), OPCODE, 0, .value.assembleFunction = assembleBranch},
      {strptr("bgt"), OPCODE, 0, .value.assembleFunction = assembleBranch},
      {strptr("ble"), OPCODE, 0, .value.assembleFunction = assembleBranch},
      {strptr("b"), OPCODE, 0, .value.assembleFunction = assembleBranch},
      {strptr("lsl"), OPCODE, 0, .value.assembleFunction = assembleDPI},
      {strptr("andeq"), OPCODE, 0, .value.assembleFunction = assembleDPI}};
  for (int i = 0; i < SYMBOLS; i++) {
    symbols[i][0] = definedSymbols[i];
  }
  add(symbolTable, symbols);
  ArmLines *armLines = initArmLines();

  FILE *armFile = fopen(argv[1], "r");
  firstPass(armFile, symbolTable, armLines);
  fclose(armFile);

  FILE *output = fopen(argv[2], "wb");
  secondPassLines(armLines, symbolTable, output);
  fclose(output);

  // freeSymbol(symbols);
  freeSymbolTable(symbolTable);
  freeLines(armLines);

  return EXIT_SUCCESS;
}
