#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <GameWindow.h>
int main() {
    gwlPrintVersion();
    printf("ASDADSASD");
    GameWindow* myWindow = gwlCreateWindow("My window!");
    gwlShowWindow(myWindow);
    gwlPrintGLVersion(myWindow);

    double time = gwlGetTime(myWindow);
    assert(time >= 0.0);

    _sleep(5000);

    time = gwlGetTime(myWindow);
    assert(time >= 5.0);

    gwlHideWindow(myWindow);

    _sleep(2500);

    time = gwlGetTime(myWindow);
    assert(time >= 7.5);

    gwlShowWindow(myWindow);

    _sleep(2500);

    time = gwlGetTime(myWindow);
    assert(time >= 10.0);

    gwlCleanupWindow(myWindow);

    _sleep(2000);

    return 0;
}