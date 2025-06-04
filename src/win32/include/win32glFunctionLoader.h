#ifndef _WIN32_GLFNPTR_H_
#define _WIN32_GLFNPTR_H_
#include <Windows.h>
#include <gl/GL.h>
#include <glext.h>
#include <wglext.h>

PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;

// courtesy of https://www.khronos.org/opengl/wiki/Load_OpenGL_Functions#Windows_2
static void* getAnyGlFuncAddress(const char* procName) {
    void* fnp = (void*)wglGetProcAddress(procName);
    if (
        fnp == 0 || 
        fnp == (void*)0x1 || 
        fnp == (void*)0x2 || 
        fnp == (void*)0x3 || 
        fnp == (void*)-1) {
        
        HMODULE hModule = LoadLibraryA("opengl32.dll");
        fnp = (void*)GetProcAddress(hModule, procName);
    }
    return fnp;
}

// only supports opengl versions >= 3.0... I don't see why you'd need to go lower
void loadGLProcs(void) {
    int openGLVersionMajor;
    int openGLVersionMinor;
    glGetIntegerv(GL_MAJOR_VERSION, &openGLVersionMajor);
    glGetIntegerv(GL_MINOR_VERSION, &openGLVersionMinor);
    // refer to https://www.khronos.org/opengl/wiki/Load_OpenGL_Functions#Windows_2 for loading based on gl version, etc

    // There's a lot of steps that a real library would have to do to ensure function pointer and Gl version compatibility, but if these functions are not available, then the window is useless. Therefore, I didn't bother myself with implementing those checks
    // Windows only needs these two functions for context creation anyways

    wglChoosePixelFormatARB = 
    (PFNWGLCHOOSEPIXELFORMATARBPROC)getAnyGlFuncAddress("wglChoosePixelFormatARB");
    wglCreateContextAttribsARB =
    (PFNWGLCREATECONTEXTATTRIBSARBPROC)getAnyGlFuncAddress("wglCreateContextAttribsARB");
}

#endif // _WIN32_GLFNPTR_H_