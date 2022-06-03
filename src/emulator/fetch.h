#ifndef FETCH
#define FETCH

#include "../definitions.h"
#include "binFileLoader.h"

// Performs fetch part of cycle, and updates the cpu's state.
// void fetch(State *cpuState);
// Initialises a cpu state on the heap, with the instructions from a .bin file, 
// and returns a pointer to this state struct.
State *initARMState(const char *fileName);

#endif
