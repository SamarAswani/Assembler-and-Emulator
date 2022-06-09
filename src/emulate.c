#include <stdlib.h>
#include <stdio.h>
#include "emulator/fetch.h"
#include "emulator/binFileLoader.h"
#include "emulator/decode.h"
#include "emulator/execute.h"

void printRegisters(State *state) {
    printf("Registers:\n");
    for (int i = 0; i < NUM_REGISTERS; i++) {
        if (i == PC) {
            printf("$  PC: %10i (0x%08x)\n", state->registers[i], state->registers[i]);
        } 
        else if (i == CPSR) {
            printf("$CPSR: %10i (0x%08x)\n", state->registers[i], state->registers[i]);
        } 
        else {
            printf("$%4i: %10i (0x%08x)\n", i, state->registers[i], state->registers[i]);
        }    
    }
    printf("Non-zero memory:\n");
    for (int i = 0; i < MEM_SIZE; i += 4) {
        word mem = getWord(state, i);
        word littleEndian = (mem >> 24) | ((mem >> 8) & 0x0000ff00) | ((mem << 8) & 0x00ff0000) | (mem << 24);
        if (littleEndian != 0) {
            printf("0x%08x: 0x%08x\n", i, littleEndian);
        }
    }
}

int main(int argc, char **argv) {
    State *armState = initARMState("tst02");
    while (!armState->decoded.isSet || (armState->decoded.isSet && armState->decoded.instruction)) {
        execute(armState);
        decode(armState);
        fetch(armState);
    }
    printRegisters(armState);
    destroyARMState(armState);
}
