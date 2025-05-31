#ifndef _GAMEWINDOW_CORE_H_
#define _GAMEWINDOW_CORE_H_
#include <stdint.h>
// Things I'll need
// Struct to store window state and info [X]
// Function to create a window [X]
// A way to run the window's message loop without halting the main thread [X]
// A way to queue messages so their callback functions can be called on the main thread (poll events)
// Function to show a window [X]
// Function to hide a window [X]
// Callback function for keyboard input processing (separate header?)
// Function to lock mouse to window
// Callback function for mouse input processing (separate header?)
// Function to create an opengl context on a window (separate header?)
// Function to make a window's opengl context current (separate header?)
// Function to swap buffers
// Function to clean up & destroy a window [X]

typedef struct GameWindow GameWindow;

typedef void (*PTRKEYBOARDINPUTCBFUNC)(GameWindow* window, void* inputMsg);
typedef void (*PTRMOUSEINPUTCBFUNC)(GameWindow* window, void* inputMsg);

extern void gwlPrintVersion(void);

extern GameWindow* gwlCreateWindow(const char* windowTitle);

extern void gwlShowWindow(GameWindow* window);

extern void gwlHideWindow(GameWindow* window);

extern void gwlCleanupWindow(GameWindow* window);

extern uint64_t gwlGetWindowStatus(GameWindow* window);

extern void gwlSetKeyboardInputCallback(GameWindow* window, PTRKEYBOARDINPUTCBFUNC callback);

extern void gwlSetMouseInputCallback(GameWindow* window, PTRMOUSEINPUTCBFUNC callback);

#endif // _GAMEWINDOW_PROTOTYPES_H