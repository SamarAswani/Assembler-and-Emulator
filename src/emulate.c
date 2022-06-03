#include <stdlib.h>
#include <stdio.h>
#include "emulator/fetch.h"
#include "emulator/binFileLoader.h"

int main(int argc, char **argv) {
    // Running this should print "hello world." if everything is okay.
    State *armState = initARMState("test.bin");
    printf("%s", armState->memory);
    destroyARMState(armState);
}
