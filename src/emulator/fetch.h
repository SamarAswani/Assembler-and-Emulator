#ifndef FETCH
#define FETCH

#include <stdio.h>
#include <stdlib.h>

#include "../definitions.h"
#include "binFileLoader.h"

#define BYTE 8

// Gets a word from memory.
word getWord(State *armState, word address);

// Performs fetch part of cycle, and updates the cpu's state.
void fetch(State *armState);

// Initialises a cpu state on the heap, with the instructions from a .bin file, 
// and returns a pointer to this state struct.
State *initARMState(const char *fileName);

#endif
