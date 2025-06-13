#include <GameWindow.h>

void inputCallback(GameWindow* window, gwInputEvent event) {
    if (event.eventType == gw_keyboardEvent) {
        gwlSetInputFlags(window, CAPTURE_MOUSE_BIT, 0);
    }
}

int main() {
    GameWindow* myWindow = gwlCreateWindow("OpenGL Loading Test :O \\_(:v)_/");
    gwlSetInputCallback(myWindow, inputCallback);
    gwlSetInputFlags(myWindow, CAPTURE_MOUSE_BIT, 1);
    gwlCreateOpenGLContext(&myWindow);
    gwlShowWindow(myWindow);

    while (gwlGetWindowStatus(myWindow) == GW_WINDOW_ACTIVE) {

        gwlPollEvents(myWindow);
        gwlSwapBuffers(myWindow);
    }
   
    gwlPrintGLVersion(myWindow);

    gwlCleanupWindow(myWindow);
    return 0;
}