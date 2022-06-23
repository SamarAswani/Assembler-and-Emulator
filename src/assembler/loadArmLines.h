#ifndef LOADARM_H
#define LOADARM_H

#include <stdlib.h>
#include <string.h>

#include "../definitions.h"

typedef struct {
  char **lines;
  unsigned int count;
  unsigned int max;
} ArmLines;


extern void addLine(ArmLines *file, const char *line);
extern void addLines(ArmLines *file, const char **lines, unsigned int n);
extern ArmLines *initArmLines(void);
extern char *strptr(const char *str);
extern void freeLines(ArmLines *file);

#endif
