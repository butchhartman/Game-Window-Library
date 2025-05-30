#include <stdio.h>
#include "GameWindowPrototypes.h"

#ifdef __linux__
void printSomething() {
    printf("Something!\n");
}
#endif // __linux__