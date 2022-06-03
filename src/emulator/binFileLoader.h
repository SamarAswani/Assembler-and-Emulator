#ifndef BINFILELOADER
#define BINFILELOADER

#include <stdio.h>
#include <stdlib.h>

#include "../definitions.h"

// Malloc
State *createCPUState(void);
// Free
void destroyCPUState(State *cpuState);
// Reads a given .bin file into a State struct.
void loadBinFile(const char *fileName, State *cpuState);

#endif
