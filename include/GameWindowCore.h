#ifndef _GAMEWINDOW_CORE_H_
#define _GAMEWINDOW_CORE_H_
#include <stdint.h>
#include <stdalign.h>

/**
 *  Identifier for the type of event recieved by the input callback function
 */
typedef enum gwEventType {gw_windowReizeEvent, gw_keyboardEvent, gw_mouseEvent} gwEventType;
/**
 * Identifier for the key pressed when a keyboard input is recieved
 */
typedef enum gwEventKeycode  {gw_W, gw_A, gw_S, gw_D, gw_KEYUNSUPPORED} gwEventKeycode;
/**
 * Identifier for the type of input when a mouse input is recieved
 */
typedef enum gwMouseEventInputCode {gw_NONE, gw_MOVE, gw_LMB, gw_RMB} gwMouseEventInputCode;

/**
 * 64 bit unsigned integer representing a bitfield of describing the state
 * of the recieved keyboard input
 */
typedef uint64_t keyStateFlags;
/**
 * 64 bit unsigned integer representing a bitfield describing the state of the recieved mouse input
 */
typedef uint64_t mouseInputStateFlags;

/**
 * Simple enum meant to reduce magic numbers in code. Describes whether or not a
 * window is active. Activity is defined by a window being visible and
 * processing events.
 */
typedef enum gwWindowStatus {
    GW_WINDOW_INACTIVE,
    GW_WINDOW_ACTIVE
} gwWindowStatus;

/**
 * Flags which describe window-wide behavior regarding inputs
 */
typedef enum inputFlagBits {
    CAPTURE_MOUSE_BIT = 0x0000000000000001, // bit 1
} inputFlagBits;

/**
 * Flags which describe information about a key input
 */
typedef enum KeyStateFlagBits{
    KEY_UP_BIT = 0x0000000000000000, // bit 1
    KEY_DOWN_BIT = 0x0000000000000001, // bit 1
} KeyStateFlagBits;

/**
 * Flags which describe information about a mouse input
 */
typedef enum mouseInputStateFlagBits {
    MOUSE_BUTTON_UP_BIT = 0x0000000000000000, // bit 1
    MOUSE_BUTTON_DOWN_BIT = 0x0000000000000001, // bit 1
} mouseInputStateFlagBits;

/**
 * Struct that the input callback will recieve and need to parse in order to
 * determine what kind of input was recieved and the state of that input.
 */
typedef struct gwInputEvent {
// TODO: Consider shortening this struct by making all fields generic and their expected values dependant on the event type
    gwEventType eventType;

    // window resize
    uint32_t windowWidth;
    uint32_t windowHeight;

    // keyboard
    gwEventKeycode key;
    keyStateFlags keyStateFlags;
    
    // mouse
    gwMouseEventInputCode mouseInputCode;
    uint32_t padding1;
    mouseInputStateFlags mouseInputStateFlags;
    int64_t xPos;
    int64_t yPos;
} gwInputEvent;

/**
 * Opaque struct object which represents the window which the user will interact with
 */
typedef struct GameWindow GameWindow;

/**
 * Function pointer type describing the expected signature of the input callback function
 */
typedef void (*PTRINPUTCBFUNC)(GameWindow* window, gwInputEvent inputEvent);

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Prints a string stating the current library version that is in use, followed by a new line
 */
extern void gwlPrintVersion(void);

/**
 * Returns the time in seconds since the passed window was created
 */
extern double gwlGetTime(GameWindow* window);

/**
 * Prints the version of the passed window's OpenGL context
 */
extern void gwlPrintGLVersion(GameWindow* window);

/**
 * Creates and returns a pointer to a GameWindow object
 * \param windowTitle The title that will be displayed on the window frame
 */
extern GameWindow* gwlCreateWindow(const char* windowTitle);

/**
 * Makes visible and enabled input processing for the passed window
 * \param window The window to be displayed
 */
extern void gwlShowWindow(GameWindow* window);

/**
 * Hides and disables input processing for the passed window 
 * \note The window does not process or queue any new inputs in this state, it is inert.
 * \param window The window to be hidden
 */
extern void gwlHideWindow(GameWindow* window);

/**
 * Destroys the passed window and frees all of its resources
 * \param window The window to be destroyed
 */
extern void gwlCleanupWindow(GameWindow* window);

/**
 * Returns if the window is active or not
 * \param window - The window whose status will be queried
 * \return GW_WINDOW_ACTIVE if active, GW_WINDOW_INACTIVE if inactive
 */
extern uint64_t gwlGetWindowStatus(GameWindow* window);

/**
 * Flushes the passed window's event queue, processing all its events
 * \param window The window whose events will be processed
 */
extern void gwlPollEvents(GameWindow* window);

/**
 * Sets the passed callback as the function the window will call whenever an
 * input event is processed
 * \param window The window whose callback will be set
 * \param callback The input callback function to set
 */
extern void gwlSetInputCallback(GameWindow* window, PTRINPUTCBFUNC callback);

/**
 * Changes the window's global input processing behavior based on the passed flags
 * \param window The window whose flags will be modified
 * \param flags The flags to modify
 * \param state The state the flags will take (1 for active, 0 for inactive)
 * \note All of a window's flags are set to inactive by default. This function
 * does not modifiy any other flags besides the ones passed to the flags
 * parameter
 */
extern void gwlSetInputFlags(GameWindow* window, inputFlagBits flags, int8_t state);

/**
 * Creates an OpenGL context for the passed window
 * \param window A pointer to the gamewindow that will be used to create the context
 * \note This function MAY be forced to re-create the passed window, hence the double pointer. 
 */
extern void gwlCreateOpenGLContext(GameWindow** window);

/**
 * Swaps the framebuffers of the passed window
 * \param window The window whose framebuffers will be swapped
 */
extern void gwlSwapBuffers(GameWindow* window);

#ifdef __cplusplus
};
#endif

#endif // _GAMEWINDOW_PROTOTYPES_H