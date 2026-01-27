#include "WndProc.hpp"

#include "glad/glad.h"

HDC hdc;
HGLRC rc;

static void InitOpenGL(HWND hWnd) {
    hdc = GetDC(hWnd);

    // Pixel format setup
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),                              // Size fof PFD
        1,                                                          // DOn't know this property
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, // Flags
        PFD_TYPE_RGBA,                                              // Pixel Type
        32,                                                         // Bits for colors excluding alpha but for memory allighment use 32 instead of 24
        0, 0, 0, 0, 0, 0,                                           // color bit and shift for rgb respectively
        0, 0,                                                       // alpha bit and shift
        0,                                                          // accumulation bits // QUESTION: Not sure what it is and how it affects
        0, 0, 0, 0,                                                 // accum rgba bits respectvily
        24,                                                         // Depth bits
        8,                                                          // Stencil bits
        0,                                                          // cAuxBuffers
        PFD_MAIN_PLANE,                                             // iLayerType
        0,                                                          // bReserved
        0, 0, 0                                                     // dwLayerMask, dwVisibleMask, dwDamageMask
    };
    int pixelFormat = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pixelFormat, &pfd);

    // OpenGL Rendering context setup
    rc = wglCreateContext(hdc);
    wglMakeCurrent(hdc, rc);

    if (!gladLoadGL())
    {
        MessageBox(hWnd, "Failed to initialize GLAD", "OpenGL Error", MB_OK | MB_ICONERROR);
        PostQuitMessage(0);
    }

    RECT rect;
    if (GetClientRect(hWnd, &rect))
    {
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;
        if (width > 0 && height > 0)
        {
            glViewport(0, 0, width, height);
        }
    }

}

LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
    case WM_CREATE:
        InitOpenGL(hWnd);
        SetTimer(hWnd, 1, 1, NULL);
        break;
    case WM_TIMER:
        InvalidateRect(hWnd, nullptr, FALSE);
        break;
	case WM_CLOSE:
        KillTimer(hWnd, 1);
		DestroyWindow(hWnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
