#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <stdlib.h>
#include <string.h>


#include "../definitions.h"


typedef enum { LABEL, OPCODE } SymbolType;
typedef struct SymbolTable SymbolTable;
typedef struct Symbol Symbol;

typedef struct Instruction {
    char *opcode;
    int mnemonic;
    char **operands;
    unsigned int opCount;
    word address;
} Instruction;

// SymbolTable ADT implemented as a binary tree map.
struct SymbolTable {
    Symbol *root;
};

struct Symbol {
    char *key;            // Opcode mnemonic or label
    SymbolType type;
    union {
        word address;     // Used to translate labels into addresses
        // Function pointer to the opcode mnemonic's assemble function
        word (*assembleFunction)(SymbolTable *, Instruction); 
    } value;
    struct Symbol *left;
    struct Symbol *right;
};

SymbolTable *createSymbolTable(void);
Symbol *createLabelSymbol(const char *key, word value);
void freeSymbolTable(SymbolTable *table);
void add(SymbolTable *table, Symbol *entry);
Symbol *get(SymbolTable *table, const char *key);

#endif
