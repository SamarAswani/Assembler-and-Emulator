#include <stdlib.h>
#include <stdio.h>
#include "emulator/fetch.h"
#include "emulator/binFileLoader.h"
#include "emulator/decode.h"

int main(int argc, char **argv) {
    State *armState = initARMState("tst02");
    fetch(armState);
    printf("%x\n", armState->fetched);
    decode(armState);
    printf("%x\n", armState->decoded.i.dp.opcode);
    printf("%x\n", armState->decoded.isSet);
    printf("%x\n", armState->decoded.instruction);
    destroyARMState(armState);
}
