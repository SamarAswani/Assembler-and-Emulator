#include "binFileLoader.h"

State *createCPUState(void) {
    State *newState = malloc(sizeof(State));
    if (newState == NULL) {
        perror("Error allocating memory for cpu state.");
        exit(EXIT_FAILURE);
    }
    return newState;
}

void destroyCPUState(State *cpuState) {
    free(cpuState);
}

void loadBinFile(const char *fileName, State *cpuState) {
    FILE *file = fopen(fileName, "rb");

    if (ferror(file)) {
        perror("Error reading input file");
        exit(EXIT_FAILURE);
    }

    fread(cpuState->memory, sizeof(byte), MEM_SIZE, file);
}
