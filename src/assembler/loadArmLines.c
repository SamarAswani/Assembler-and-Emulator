#include "loadArmLines.h"
#include "symbolTable.h"
#include "../definitions.h"
#include "constants.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *strptr(const char *str) {
    char *ptr = malloc(strlen(str) + 1);
    strcpy(ptr, str);
    return ptr;
}

void resize(ArmLines *file){
    file->lines = realloc(file->lines, file->max *= 2);
}

void addLine(ArmLines *file, char *line) {
    if (file->count == file->max) {
      resize(file);
    }
    file->count++;
    file->lines[file->count] = strptr(line);
}

void addLines(ArmLines *file, char **lines, unsigned int n) {
    for (int i = 0; i < n; i++) {
      addLine(file, lines[i]);
    }
}

void printLines(ArmLines *file) {
    for (int i = 1; i <= file->count; i++) {
      printf("%s\n", file->lines[i]);
    }
}

ArmLines *initArmLines(void) {
    ArmLines *file = malloc(sizeof(*file));
    file->lines = malloc(sizeof(*file->lines) * INITIAL_SIZE);
    file->max = INITIAL_SIZE;
    file->count = 0;
    return file;
}

void freeLines(ArmLines *file) {
    for (int i = 0; i < file->count; i++) {
      free(file->lines[i]);
    }
    free(file->lines);
    free(file);
}