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

    while (gwlGetWindowStatus(myWindow) == 1) {

        gwlPollEvents(myWindow);
        gwlSwapBuffers(myWindow);
    }
   
    gwlCleanupWindow(myWindow);
    return 0;
}