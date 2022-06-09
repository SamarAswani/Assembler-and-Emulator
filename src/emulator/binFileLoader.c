#include "binFileLoader.h"

State *createARMState(void) {
    State *newState = malloc(sizeof(State));
    if (newState == NULL) {
        perror("Error allocating memory for cpu state.");
        exit(EXIT_FAILURE);
    }
    return newState;
}

void destroyARMState(State *armState) {
    free(armState);
}

void loadBinFile(const char *fileName, State *armState) {
    FILE *file = fopen(fileName, "rb");

    if (ferror(file)) {
        destroyARMState(armState);
        perror("Error reading input file");
        exit(EXIT_FAILURE);
    }

    fread(armState->memory, sizeof(byte), MEM_SIZE, file);
    fclose(file);
}
