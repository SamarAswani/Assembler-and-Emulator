#include "loadArmLines.h"
#include "symbolTable.h"
#include "../definitions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *strptr(char *str) {
    char *ptr = malloc(strlen(str) + 1);
    strcpy(ptr, str);
    return ptr;
}

void resize(File *file){
    file->lines = realloc(file->lines, file->max *= 2);
}

void addLine(File *file, char *line) {
    if (file->count == file->max) {
      resize(file);
    }
    file->count++;
    file->lines[file->count] = strptr(line);
}

void addLines(File *file, char **lines, unsigned int n) {
    for (int i = 0; i < n; i++) {
      addLine(file, lines[i]);
    }
}