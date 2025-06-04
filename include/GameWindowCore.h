#ifndef _GAMEWINDOW_CORE_H_
#define _GAMEWINDOW_CORE_H_
#include <stdint.h>
#include <stdalign.h>
// Things I'll need
// Struct to store window state and info [X]
// Function to create a window [X]
// A way to run the window's message loop without halting the main thread [X]
// A way to queue messages so their callback functions can be called on the main thread (poll events) [X]
// Function to show a window [X]
// Function to hide a window [X]
// Callback function for keyboard input processing [X]
// Function to lock mouse to window [X]
// Callback function for mouse input processing [X] 
// Function to create an opengl context on a window [X] 
// Function to make a window's opengl context current  -- is done automatically, which means that having multiple windows with different contexts is unsupported
// Function to swap buffers [X]
// Function to clean up & destroy a window [X]

typedef enum gwEventType {gw_windowReizeEvent, gw_keyboardEvent, gw_mouseEvent} gwEventType;
typedef enum gwEventKeycode  {gw_W, gw_A, gw_S, gw_D, gw_KEYUNSUPPORED} gwEventKeycode;
typedef enum gwMouseEventInputCode {gw_NONE, gw_MOVE, gw_LMB, gw_RMB} gwMouseEventInputCode;

// bit 1 is up/down
typedef uint64_t keyStateFlags;
typedef uint64_t mouseInputStateFlags;

typedef enum gwWindowStatus {
    GW_WINDOW_INACTIVE,
    GW_WINDOW_ACTIVE
} gwWindowStatus;

typedef enum inputFlagBits {
    CAPTURE_MOUSE_BIT = 0x0000000000000001, // bit 1
} inputFlagBits;

typedef enum KeyStateFlagBits{
    KEY_UP_BIT = 0x0000000000000000, // bit 1
    KEY_DOWN_BIT = 0x0000000000000001, // bit 1
} KeyStateFlagBits;

typedef enum mouseInputStateFlagBits {
    MOUSE_BUTTON_UP_BIT = 0x0000000000000000, // bit 1
    MOUSE_BUTTON_DOWN_BIT = 0x0000000000000001, // bit 1
} mouseInputStateFlagBits;

// TODO: Consider shortening this struct by making all fields generic and their expected values dependant on the event type
typedef struct gwInputEvent {
    gwEventType eventType;

    // window resize
    uint32_t windowWidth;
    uint32_t windowHeight;

    // keyboard
    gwEventKeycode key;
    keyStateFlags keyStateFlags;
    
    // mouse
    // lmb/rmb, up/down,
    gwMouseEventInputCode mouseInputCode;
    mouseInputStateFlags mouseInputStateFlags;
    int64_t xPos;
    int64_t yPos;
} gwInputEvent;

typedef struct GameWindow GameWindow;

typedef void (*PTRINPUTCBFUNC)(GameWindow* window, gwInputEvent inputEvent);

#ifdef __cplusplus
extern "C" {
#endif


extern void gwlPrintVersion(void);

extern GameWindow* gwlCreateWindow(const char* windowTitle);

extern void gwlShowWindow(GameWindow* window);

extern void gwlHideWindow(GameWindow* window);

extern void gwlCleanupWindow(GameWindow* window);

extern uint64_t gwlGetWindowStatus(GameWindow* window);

extern void gwlPollEvents(GameWindow* window);

extern void gwlSetInputCallback(GameWindow* window, PTRINPUTCBFUNC callback);

extern void gwlSetInputFlags(GameWindow* window, inputFlagBits flags, int8_t state);

extern void gwlCreateOpenGLContext(GameWindow** window);

extern void gwlSwapBuffers(GameWindow* window);

#ifdef __cplusplus
}
#endif

#endif // _GAMEWINDOW_PROTOTYPES_H