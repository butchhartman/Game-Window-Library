typedef int make_iso_compilers_happy;

#ifdef __linux__
#include <stdio.h>
#include "GameWindowCore.h"

void printSomething() {
    printf("Something!\n");
}
#endif // __linux__