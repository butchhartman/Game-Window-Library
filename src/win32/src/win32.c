#include <stdio.h>
#include "GameWindowPrototypes.h"

#ifdef _WIN32

typedef struct GameWindow {
    float a;
    float b;
} GameWindow;

void printVersion() {
    printf("Running GWL version %s", GWL_VERSION);
}
#endif // _WIN32