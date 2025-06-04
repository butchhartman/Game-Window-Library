typedef int make_iso_compilers_happy;

#ifdef _WIN32

#ifndef UNICODE
#define UNICODE
#endif // UNICODE

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <Windows.h>
#include <windowsx.h>
#include <GameWindowCore.h>
#include <datastructures/GameEventQueue.h>
#include <gl/GL.h>
#include <glextensions/glext.h>
#include <glextensions/wglext.h>
#include <win32glFunctionLoader.h>

// make this only defined if debug is defined?
#define GWL_LOG(message) { \
   printf("(%s - Line %d): %s\n", __FILE__, __LINE__, message);\
} \

static gwEventKeycode translateWparamToKeycode(WPARAM wParam);

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static DWORD WINAPI initializeHwnd(GameWindow* window);


typedef struct GameWindow {
    HWND handle;
    HANDLE windowMainThread;
    DWORD windowMainThreadID;
    wchar_t* windowTitle;
    PTRINPUTCBFUNC inputCallback;
    geQueue eventQueue;
    HGLRC openGLContext;
    HDC deviceContext;
    uint64_t isActive;
    inputFlagBits inputFlags;
    int64_t windowXPos;
    int64_t windowYPos;
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
    newWindow->openGLContext = NULL;
    newWindow->isActive = FALSE;
    newWindow->inputCallback = NULL;
    newWindow->inputFlags = 0;
    newWindow->windowXPos = 0;
    newWindow->windowYPos = 0;
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

    // destroy opengl resources
    if (window->openGLContext != NULL) {
        wglMakeCurrent(window->deviceContext, NULL);
        wglDeleteContext(window->openGLContext);
    }

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
        window->inputCallback(window, event);
    }
}

void gwlSetInputCallback(GameWindow* window, PTRINPUTCBFUNC callback) {
    window->inputCallback = callback;
}

void gwlSetInputFlags(GameWindow* window, inputFlagBits flags, int8_t state) {
    if (state == 1) {
        window->inputFlags |= flags;
    } else if (state == 0) {
        window->inputFlags &= ~flags;
    }
}

void gwlCreateOpenGLContext(GameWindow** window) {
    // double pointer here because I may need to modify the original, passed window when its recreated
    (*window)->deviceContext = GetDC((*window)->handle);
    const PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL| PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA, // The kind of framebuffer
        32, // Color depth of the framebuffer
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24, // Number of bits for depth buffer
        8, // Number of bits for stencil buffer
        0, // Number of auxillary bits in the framebuffer
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    int falsePixelFormatID = ChoosePixelFormat((*window)->deviceContext, &pfd);
    // returns 0 on failure
    if (falsePixelFormatID == 0) {
        GWL_LOG("Pixel format could not be found");
    }
    SetPixelFormat((*window)->deviceContext, falsePixelFormatID, &pfd);
    HGLRC falseContext = wglCreateContext((*window)->deviceContext);
    wglMakeCurrent((*window)->deviceContext, falseContext);
    loadGLProcs();

    const int attribList[] = {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB, 32,
        WGL_DEPTH_BITS_ARB, 24,
        WGL_STENCIL_BITS_ARB, 8,
        0, // 0 designates end
    };

    int pixelFormat;
    unsigned int numFormats;

    int wglPixelFormatResult = 
    wglChoosePixelFormatARB((*window)->deviceContext, attribList, NULL, 1, &pixelFormat, &numFormats);

    if (wglPixelFormatResult == FALSE) {
        GWL_LOG("wglChoosePixelFormatARB failed to find pixel format");
    }
    // you can only set pixel format once per window, so I must recreate a window here if two differing formats are found
    if (pixelFormat != falsePixelFormatID) {
        // seems to work!
        // recreate window
        
        // ugly stupid way to handle keeping the name the same
        // converts wchar_t* back to char*
        size_t origLength = wcslen((*window)->windowTitle) + 1;
        size_t convertedChars = 0;

        // *2 because multibyte can take 2 bytes per char 
        size_t mbSize = sizeof(char) * origLength * 2; 
        char* mbOrigCopy = malloc(mbSize);
        wcstombs_s(&convertedChars, mbOrigCopy, mbSize, (*window)->windowTitle, _TRUNCATE);

        GameWindow* newWind = gwlCreateWindow(mbOrigCopy); // FUCK YEAH!!!!@
        newWind->inputCallback = (*window)->inputCallback;
        newWind->inputFlags = (*window)->inputFlags;
        newWind->deviceContext = GetDC(newWind->handle);

        SetPixelFormat(newWind->deviceContext, pixelFormat, NULL);
        gwlCleanupWindow(*window);
        *window = newWind;
        free(mbOrigCopy);
    }

    const int contextAttribList[] = {// TODO : Consider making this not hardcoded
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 6,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0 // signifies end
    };

    HGLRC trueContext =
    wglCreateContextAttribsARB((*window)->deviceContext, NULL, contextAttribList);

    (*window)->openGLContext = trueContext;
    wglMakeCurrent((*window)->deviceContext, (*window)->openGLContext);
    wglDeleteContext(falseContext);
    #ifdef _DEBUG
        MessageBoxA(0, (char*)glGetString(GL_VERSION), "OPENGL VERSION", 0);
    #endif // _DEBUG
}

void gwlSwapBuffers(GameWindow* window) {
    SwapBuffers(window->deviceContext);
}

static gwEventKeycode translateWparamToKeycode(WPARAM wParam) {
    switch (wParam) {
    case 'W':
        return gw_W;
        break;
    case 'A':
        return gw_A;
        break;
    case 'S':
        return gw_S;
        break;
    case 'D':
        return gw_D;
        break;
    default:
        return gw_KEYUNSUPPORED;
        break;
    }
}

// TODO : Add support for WASD, LMB and RMB clicks, and window resizing
static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    GameWindow* window = GetProp(hwnd, L"WINDOW_STRUCT_DATA");
    // If there's no reason to add something to the queue, dont
    if (window == NULL || (window->inputCallback == NULL && uMsg != WM_DESTROY) ) {
        return DefWindowProc(hwnd, uMsg, wParam, lParam); 
    }

    gwInputEvent thisEvent;
    memset(&thisEvent, 0, sizeof(gwInputEvent));            
    thisEvent.eventType = -1;

    switch(uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            window->isActive = 0;
            break;

        case WM_KEYDOWN:

            thisEvent.eventType = gw_keyboardEvent;
            thisEvent.keyStateFlags |= KEY_DOWN_BIT;
            thisEvent.key = translateWparamToKeycode(wParam);

            break;

        case WM_MOUSEMOVE:

            thisEvent.eventType = gw_mouseEvent;
            thisEvent.mouseInputCode = gw_NONE;
            thisEvent.xPos = GET_X_LPARAM(lParam);
            thisEvent.yPos = GET_Y_LPARAM(lParam);

            if (window->inputFlags & CAPTURE_MOUSE_BIT) {
                // kind of a dumb implementation, but it works and is probably fine for performance
                RECT windowRect;
                GetWindowRect(hwnd, &windowRect);
                SetCursorPos(windowRect.right/2, windowRect.bottom/2);
            }

            break;

        case WM_SIZE:

           thisEvent.eventType = gw_windowReizeEvent;
           thisEvent.windowHeight = HIWORD(lParam);
           thisEvent.windowWidth = LOWORD(lParam);
           break;

        default:
            break;
    }

    // only queue when the event type is valid 
    if (thisEvent.eventType != -1) {
        geQueueEnqueue(&window->eventQueue, &thisEvent, sizeof(gwInputEvent));
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

static DWORD WINAPI initializeHwnd(GameWindow* window) {
    const wchar_t gameWindowClassName[] = L"Game Window";

    WNDCLASS gameWindowClass = {0};
    gameWindowClass.lpfnWndProc = WindowProc;
    gameWindowClass.hInstance = GetModuleHandle(NULL);
    gameWindowClass.lpszClassName = gameWindowClassName;

    // It technically may create issues when this is called multiple times for the same class but idc lol
    RegisterClass(&gameWindowClass); 
    // assert(classIdentifier != 0); // ensure class successfully registers

    HWND hwnd = CreateWindowEx(
        0,
        gameWindowClassName,
        window->windowTitle, // multiByteToWideChar
        WS_OVERLAPPEDWINDOW | CS_OWNDC, // CS_OWNDC is required for OpenGL. 

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