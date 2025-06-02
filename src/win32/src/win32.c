typedef int make_iso_compilers_happy;

#ifdef _WIN32

#ifndef UNICODE
#define UNICODE
#endif // UNICODE

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <Windows.h>
#include <GameWindowCore.h>
#include <datastructures/GameEventQueue.h>
// make this only defined if debug is defined?
#define GWL_LOG(message) { \
   printf("(%s - Line %d): %s\n", __FILE__, __LINE__, message);\
} \

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static DWORD WINAPI initializeHwnd(GameWindow* window);

typedef struct GameWindow {
    HWND handle;
    HANDLE windowMainThread;
    wchar_t* windowTitle;
    uint64_t isActive;
    PTRKEYBOARDINPUTCBFUNC keyboardInputCallback;
    PTRMOUSEINPUTCBFUNC mouseInputCallback;
    geQueue eventQueue;
} GameWindow;

void gwlPrintVersion(void) {
    printf("Running GWL version %s", GWL_VERSION);
}

GameWindow* gwlCreateWindow(const char* windowTitle) {
    GameWindow* newWindow = malloc(sizeof(GameWindow));
    // Convert from char string to wide char string (unicode)
    size_t titleLength = strlen(windowTitle) + 1;
    newWindow->windowTitle = malloc(titleLength * sizeof(wchar_t));
    assert(newWindow->windowTitle != NULL); // Ensure malloc was successful
    size_t convertedChars = 0;
    errno_t conversionResult = 
        mbstowcs_s(&convertedChars,
                newWindow->windowTitle, 
                titleLength,windowTitle, 
                _TRUNCATE
        );
    assert(conversionResult == 0); // ensure conversion success

    newWindow->handle = NULL;
    newWindow->isActive = FALSE;
    newWindow->keyboardInputCallback = NULL;
    newWindow->mouseInputCallback = NULL;
    geQueueCreate(&newWindow->eventQueue);
    newWindow->windowMainThread = 
        CreateThread(
            NULL,
            0,
            initializeHwnd,
            newWindow,
            0,
            NULL);
    assert(newWindow->windowMainThread != NULL); // ensure thread creation succeeds

    while (!newWindow->handle) {
        GWL_LOG("Handle not yet created");
    }

    SuspendThread(newWindow->windowMainThread);

    return newWindow;
}

void gwlShowWindow(GameWindow* window) {
    window->isActive = TRUE;
    ResumeThread(window->windowMainThread);
    ShowWindow(window->handle, SW_SHOW);
}

void gwlHideWindow(GameWindow* window) {
    window->isActive = FALSE;
    ShowWindow(window->handle, SW_HIDE);
    SuspendThread(window->windowMainThread);
}

void gwlCleanupWindow(GameWindow* window) {
    // Hide window from user
    ShowWindow(window->handle, SW_HIDE);

    // Clean up resources / states
    window->isActive = FALSE;
    free(window->windowTitle);

    // Destroy the window and free its remaining resources
    DestroyWindow(window->handle);
    free(window);
}

uint64_t gwlGetWindowStatus(GameWindow* window) {
    return window->isActive;
}

void gwlPollEvents(GameWindow* window) {
    while (!geQueueIsEmpty(&window->eventQueue)) {
        gwInputEvent event;
        geQueueDequeue(&window->eventQueue, &event);

        if (event.eventType == gw_keyboardEvent) {
            // keyboard callback
            window->keyboardInputCallback(window, event);
        } else if (event.eventType == gw_mouseEvent) {
            // mouse callback
            window->mouseInputCallback(window, event);
        }
    }
}

void gwlSetKeyboardInputCallback(GameWindow* window, PTRKEYBOARDINPUTCBFUNC callback) {
    window->keyboardInputCallback = callback;
}

void gwlSetMouseInputCallback(GameWindow* window, PTRMOUSEINPUTCBFUNC callback) {
    window->mouseInputCallback = callback;
}

// TODO : Add support for WASD, LMB and RMB clicks, and window resizing
static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    GameWindow* window = GetProp(hwnd, L"WINDOW_STRUCT_DATA");
    gwInputEvent thisEvent;
    switch(uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_KEYDOWN:
            // Ensure there is a callback to actually read from the queue
            if (window->keyboardInputCallback == NULL) { 
                break;
            }

            memset(&thisEvent, 0, sizeof(gwInputEvent));            
            thisEvent.eventType = gw_keyboardEvent;
            thisEvent.keyStateFlags |= KEY_DOWN_BIT;

            if (wParam == 'W') {
                thisEvent.key = gw_W;
            } else {
                thisEvent.key = gw_KEYUNSUPPORED;
            }

            geQueueEnqueue(&window->eventQueue, &thisEvent, sizeof(gwInputEvent));
            break;
        case WM_MOUSEMOVE:
            // Ensure there is a callback to actually read from the queue
            if (window->mouseInputCallback == NULL) {
                break;
            }
            POINT cursorPos;
            GetCursorPos(&cursorPos);

            memset(&thisEvent, 0, sizeof(gwInputEvent));            
            thisEvent.eventType = gw_mouseEvent;
            thisEvent.mouseInputCode = gw_NONE;
            thisEvent.xPos = cursorPos.x;
            thisEvent.yPos = cursorPos.y;
            geQueueEnqueue(&window->eventQueue, &thisEvent, sizeof(gwInputEvent));
            break;
        default:
            break;
    }


    return DefWindowProc(hwnd, uMsg, wParam, lParam);

}

static DWORD WINAPI initializeHwnd(GameWindow* window) {
    const wchar_t gameWindowClassName[] = L"Game Window";

    WNDCLASS gameWindowClass = {0};
    gameWindowClass.lpfnWndProc = WindowProc;
    gameWindowClass.hInstance = GetModuleHandle(NULL);
    gameWindowClass.lpszClassName = gameWindowClassName;

    ATOM classIdentifier = RegisterClass(&gameWindowClass); 
    assert(classIdentifier != 0); // ensure class successfully registers

    HWND hwnd = CreateWindowEx(
        0,
        gameWindowClassName,
        window->windowTitle, // multiByteToWideChar
        WS_OVERLAPPEDWINDOW,

        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,
        NULL,
        GetModuleHandle(NULL),
        NULL
    );

    assert(hwnd != NULL); // Ensure hwnd successfully initializes

    SetProp(hwnd, L"WINDOW_STRUCT_DATA", window);
    window->handle = hwnd;

    MSG msg = {0};
    while (GetMessage(&msg, hwnd, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}


#endif // _WIN32