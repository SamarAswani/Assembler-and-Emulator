#include <stdlib.h>
#include <stdio.h>
#include "emulator/fetch.h"
#include "emulator/binFileLoader.h"
#include "emulator/decode.h"

int main(int argc, char **argv) {
    State *armState = initARMState("tst02");
    fetch(armState);
    DecodedInstruction *decoded = malloc(sizeof(*decoded));
    printf("%x\n", armState->fetched);
    decode(armState, decoded);
    destroyARMState(armState);
}
