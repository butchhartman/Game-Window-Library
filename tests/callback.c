#include <stdio.h>
#include <stdlib.h>
#include <GameWindow.h>

int hitKey = 0;
int moust = 0;
int mouseclick = 0;

void inputCallback(GameWindow* window, gwInputEvent msg) {
    printf("callbackproc\n");
    printf("%d\n", msg.eventType);
    if (msg.eventType == gw_keyboardEvent && msg.keyStateFlags | KEY_DOWN_BIT) {
        printf("PRESSED KEY: %d\n", msg.key);
        hitKey = 1;
    } else if (hitKey == 1 && msg.eventType == gw_mouseEvent) {
        printf("MOUSE X POS: %d, MOUSE Y POS: %d\n", msg.xPos, msg.yPos);
        moust = 1;
        if (msg.mouseInputCode == gw_LMB && msg.mouseInputStateFlags & MOUSE_BUTTON_DOWN_BIT) {
            mouseclick = 1;
        }
    } else if (hitKey == 0 && moust == 0 && msg.eventType == gw_windowReizeEvent) {
        printf("WINDOW WIDTH: %d, WINDOW HEIGHT: %d\n", msg.windowWidth, msg.windowHeight);
    } 
}

// It is not tested whether or not the callbacks are actually run in the main thread, but they should be.
// Currently, this only tests for whether or not the callback functions will actually be called. It does not test if the passed conditions are correct or if the callback is called in the main thread (intended behavior) yet.
// To make this test pass, the tester must first select the window and press a key, then hover their mouse over the window, then click LMB
int main() {

    GameWindow* myWindow = gwlCreateWindow("Event queue and callback testing!!!");
    gwlSetInputCallback(myWindow, inputCallback);

    gwlShowWindow(myWindow);

    while (gwlGetWindowStatus(myWindow) == 1) {
        /* code */
        if (hitKey == 1) {
            gwlHideWindow(myWindow);
        }
        gwlPollEvents(myWindow);
    }

    gwlShowWindow(myWindow);
    while (gwlGetWindowStatus(myWindow)) {
        /* code */
        if (moust == 1 && mouseclick == 1) {
            gwlHideWindow(myWindow);
        }
        gwlPollEvents(myWindow);
    }

    return 0;
}