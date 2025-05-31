#include <stdlib.h>
#include <stdio.h>
#include <GameWindow.h>
int main() {
    printVersion();
    printf("ASDADSASD");
    GameWindow* myWindow = gwlCreateWindow("My window!");
    gwlShowWindow(myWindow);

    _sleep(5000);

    gwlHideWindow(myWindow);

    _sleep(2500);

    gwlShowWindow(myWindow);

    _sleep(2500);

    gwlCleanupWindow(myWindow);

    _sleep(2000);

    return 0;
}