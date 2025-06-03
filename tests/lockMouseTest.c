#include <GameWindow.h>

void inputCallback(GameWindow* window, gwInputEvent event) {
    if (event.eventType == gw_keyboardEvent) {
        gwlSetInputFlags(window, CAPTURE_MOUSE_BIT, 0);
    }
}

int main() {
    GameWindow* myWindow = gwlCreateWindow("Mouse locking test");
    gwlSetInputCallback(myWindow, inputCallback);
    gwlSetInputFlags(myWindow, CAPTURE_MOUSE_BIT, 1);
    gwlShowWindow(myWindow);

    while (gwlGetWindowStatus(myWindow) == 1) {
        gwlPollEvents(myWindow);
    }

    return 0;
}