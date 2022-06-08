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

    // Initialise fetched and decoded instructions
    newState->decoded.isSet = false;
    newState->decoded.instruction = 0;
    newState->fetched = 0;

    // Loads binary file into memory:
    loadBinFile(fileName, newState);

    return newState;
}

word getWord(State *armState, word address) {
    if (address + 3 >= MEM_SIZE) {
        perror("Tried to access memory out of bounds.");
        exit(EXIT_FAILURE);
    }

    word fetched = 0;

    for (int i = 0; i < WORD_IN_BYTES; i ++) {
        fetched += armState->memory[address + i] << (i * BYTE);
    }
    
    return fetched;
}

void fetch(State *armState) {
    word pc = armState->registers[PC];
    armState->fetched = getWord(armState, pc);
    armState->registers[PC] += 4;
}
