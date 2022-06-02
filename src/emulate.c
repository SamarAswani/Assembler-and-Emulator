#include <stdlib.h>
#include "binFileLoader.h"

int main(int argc, char **argv) {
    // Testing binFileLoader - create a test.bin file before running.
    State *test = createCPUState();
    loadBinFile("test.bin", test);
    destroyCPUState(test);


    return EXIT_SUCCESS;
}
