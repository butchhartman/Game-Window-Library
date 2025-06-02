#include <stdio.h>
#include <stdlib.h>
#include <GameWindow.h>

int hitKey = 0;

void keyboardCallback(GameWindow* window, gwInputEvent msg) {
    printf("Keyboard callback\n");
    printf("%d\n", msg.eventType);
    hitKey = 1;
}

void mouseCallback(GameWindow* window, gwInputEvent msg) {
    printf("Mouse callback\n");
    printf("MOUSE X: %d, MOUSE Y: %d\n", msg.xPos, msg.yPos);
    hitKey = 1;
}
// It is not tested whether or not the callbacks are actually run in the main thread, but they should be.
// Currently, this only tests for whether or not the callback functions will actually be called. It does not test if the passed conditions are correct or if the callback is called in the main thread (intended behavior) yet.
// To make this test pass, the tester must first select the window and press a key, then hover their mouse over the window
int main() {

    GameWindow* myWindow = gwlCreateWindow("Event queue and callback testing!!!");
    gwlSetKeyboardInputCallback(myWindow, keyboardCallback);

    gwlShowWindow(myWindow);

    while (gwlGetWindowStatus(myWindow) == 1) {
        /* code */
        if (hitKey == 1) {
            gwlHideWindow(myWindow);
        }
        gwlPollEvents(myWindow);
    }
    hitKey = 0;
    gwlSetMouseInputCallback(myWindow, mouseCallback);
    gwlSetKeyboardInputCallback(myWindow, NULL);

    gwlShowWindow(myWindow);
    while (gwlGetWindowStatus(myWindow) == 1) {
        /* code */
        if (hitKey == 1) {
            gwlHideWindow(myWindow);
        }
        gwlPollEvents(myWindow);
    }

    return 0;
}