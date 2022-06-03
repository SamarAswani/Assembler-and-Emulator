#ifndef BINFILELOADER
#define BINFILELOADER

#include <stdio.h>
#include <stdlib.h>

#include "../definitions.h"

// Malloc
State *createARMState(void);
// Free
void destroyARMState(State *armState);
// Reads a given .bin file into a State struct.
void loadBinFile(const char *fileName, State *armState);

#endif
