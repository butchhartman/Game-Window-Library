#ifdef _WIN32

#ifndef UNICODE
#define UNICODE
#endif // UNICODE

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "GameWindowPrototypes.h"
#include <Windows.h>

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static DWORD WINAPI initializeHwnd(GameWindow* window);

typedef struct GameWindow {
    HWND handle;
    const char* windowTitle;
    uint8_t isActive;
    HANDLE windowMainThread;
} GameWindow;

void printVersion(void) {
    printf("Running GWL version %s", GWL_VERSION);
}

GameWindow* gwlCreateWindow(const char* windowTitle) {
    GameWindow* newWindow = malloc(sizeof(GameWindow));
    newWindow->handle = NULL;
    newWindow->windowTitle = windowTitle;
    newWindow->isActive = FALSE;
    newWindow->windowMainThread = 
        CreateThread(
            NULL,
            0,
            initializeHwnd,
            newWindow,
            0,
            NULL);

    while (!newWindow->handle) {
        printf("Handle not yet created\n");
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
    window->isActive = FALSE;
    ShowWindow(window->handle, SW_HIDE);
    DestroyWindow(window->handle);
    free(window);
}

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch(uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
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

    assert(RegisterClass(&gameWindowClass) != 0); // ensure class successfully registers

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

    window->handle = hwnd;

    MSG msg = {0};
    while (GetMessage(&msg, hwnd, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}


#endif // _WIN32