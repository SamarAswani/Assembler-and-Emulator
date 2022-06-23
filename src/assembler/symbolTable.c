#include "symbolTable.h"

SymbolTable *createSymbolTable(void) {
    SymbolTable *new = malloc(sizeof(SymbolTable));
    new->root = NULL;
    return new;
}

Symbol *createLabelSymbol(const char *key, word value) {
    Symbol *new = malloc(sizeof(Symbol));

    // Stores a copy of the key.
    char *copy = malloc(sizeof(char) * (strlen(key) + 1));
    strcpy(copy, key);
    new->key = copy;

    new->type = LABEL;
    new->value.address = value;
    new->left = NULL;
    new->right = NULL;
    return new;
}

void freeSymbol(Symbol *symbol) {
    if (symbol == NULL) {
        return;
    }

    freeSymbol(symbol->left);
    freeSymbol(symbol->right);
    free(symbol);
}

void freeSymbolTable(SymbolTable *table) {
    freeSymbol(table->root);
    free(table);
}

Symbol *addSymbol(Symbol *curr, Symbol *entry) {
    if (curr == NULL) {
        return entry;
    }

    int comparison = strcmp(entry->key, curr->key);
    if (comparison == 0) {
        return curr;
    } else if (comparison < 0) {
        curr->left = addSymbol(curr->left, entry);
        return curr;
    } else {
        curr->right = addSymbol(curr->right, entry);
        return curr;
    }
}

void add(SymbolTable *table, Symbol *entry) {
    table->root = addSymbol(table->root, entry);
}

Symbol *getSymbol(Symbol *curr, const char *key) {
    if (curr == NULL) {
        return NULL;
    }

    int comparison = strcmp(key, curr->key);
    if (comparison == 0) {
        return curr;
    } else if (comparison < 0) {
        return getSymbol(curr->left, key);
    } else {
        return getSymbol(curr->right, key);
    }
}

Symbol *get(SymbolTable *table, const char *key) {
    // Returns NULL if key is not found.
    return getSymbol(table->root, key);
}
