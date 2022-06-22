#include <stdio.h>
#include <stdlib.h>


void ptrCheck(const void *ptr) {
    if (ptr == NULL) {
        printf("Error: NULL pointer.");
        exit(0);
    }
}

void throwPrint(const char *message) {
    printf("Error: %s", message);
    exit(0);
}