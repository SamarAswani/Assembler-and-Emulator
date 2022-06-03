#include "fetch.h"

State *initARMState(const char *fileName) {
    // Creates a new state:
    State *newState = createARMState();

    // Initialise memory and registers to 0:
    for (int i = 0; i < MEM_SIZE; i ++) {
        newState->memory[i] = 0;
    }

    for (int i = 0; i < NUM_REGISTERS; i ++) {
        newState->registers[i] = 0;
    }

    // Loads binary file into memory:
    loadBinFile(fileName, newState);

    return newState;
}

