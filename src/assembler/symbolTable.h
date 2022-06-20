#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <stdlib.h>
#include <string.h>

#include "../definitions.h"


typedef enum { LABEL, OPCODE } SymbolType;

typedef struct Instruction {
    char *opcode;
    int mnemonic;
    char **operands;
    unsigned int opCount;
    word address;
} Instruction;

typedef struct Symbol {
    char *key;            // Opcode mnemonic or label
    SymbolType type;
    union {
        word address;     // Used to translate labels into addresses
        // Function pointer - Used to determine which function is used 
        //                    to assemble a particular opcode mnemonic.
    } value;
    struct Symbol *left;
    struct Symbol *right;
} Symbol;

// SymbolTable ADT implemented as a binary tree map.
typedef struct SymbolTable {
    Symbol *root;
} SymbolTable;

SymbolTable *createSymbolTable(void);
Symbol *createLabelSymbol(const char *key, word value);
void freeSymbolTable(SymbolTable *table);
void add(SymbolTable *table, Symbol *entry);
Symbol *get(SymbolTable *table, const char *key);


#endif
