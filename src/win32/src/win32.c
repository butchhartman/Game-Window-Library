#include <stdio.h>
#include "GameWindowPrototypes.h"

#ifdef _WIN32

typedef struct GameWindow {
    float a;
    float b;
} GameWindow;

void printSomething() {
    printf("Something!\n");
}
#endif // _WIN32