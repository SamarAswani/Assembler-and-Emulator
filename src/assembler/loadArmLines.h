#ifndef LOADARM_H
#define LOADARM_H

#include <stdlib.h>
#include <string.h>

#include "../definitions.h"

typedef struct {
  char **lines;
  unsigned int count;
  unsigned int max;
} File;

extern char *strptr(const char *str);

#endif