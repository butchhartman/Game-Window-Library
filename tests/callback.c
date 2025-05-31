#include <stdio.h>
#include <stdlib.h>
#include <GameWindow.h>

int hitKey = 0;

void keyboardCallback(GameWindow* window, void* msg) {
    printf("Keyboard callback");
    hitKey = 1;
}

void mouseCallback(GameWindow* window, void* msg) {
    printf("Mouse callback");
    hitKey = 1;
}
// Currently, this only tests for whether or not the callback functions will actually be called. It does not test if the passed conditions are correct or if the callback is called in the main thread (intended behavior) yet.
// To make this test pass, the tester must first select the window and press a key, then hover their mouse over the window
int main() {
    GameWindow* myWindow = gwlCreateWindow("Callback Testing!!");
    gwlSetKeyboardInputCallback(myWindow, keyboardCallback);
    gwlShowWindow(myWindow);
    while (hitKey == 0) {
        printf("Waiting for keyboard...");
    }
    gwlHideWindow(myWindow);

    hitKey = 0;
    gwlSetKeyboardInputCallback(myWindow, NULL);
    gwlSetMouseInputCallback(myWindow, mouseCallback);
    gwlShowWindow(myWindow);
    while (hitKey == 0) {
        printf("Waiting for mouse...");
    }

    gwlCleanupWindow(myWindow);

    return 0;
}