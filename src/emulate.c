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
}

int main(int argc, char **argv) {
    State *armState = initARMState("tst02");
    // fetch(armState);
    // printf("%x\n", armState->fetched);
    // decode(armState);
    while ((!armState->decoded.isSet || (armState->decoded.isSet && !(armState->decoded.instruction == 0))) 
        && armState->registers[PC] < MEM_SIZE) {
        execute(armState);
        decode(armState);
        fetch(armState);
    }
    // printf("%x\n", armState->decoded.i.dp.opcode);
    // printf("%x\n", armState->decoded.isSet);
    // printf("%x\n", armState->decoded.instruction);
    printRegisters(armState);
    destroyARMState(armState);
}
