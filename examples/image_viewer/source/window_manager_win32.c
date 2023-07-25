
#include "window_manager.h"

#include <stdio.h>
#include <Windows.h>

struct graphics_buffer {
    HBITMAP hbm;
    unsigned int* data;
};
typedef struct graphics_buffer graphics_buffer;

static const char* g_window_class_name = "LPM Image Viewer Window Class";
static const char* g_window_title = "libpicomedia: Image Viewer";
static HWND g_window_handle = NULL;
static HINSTANCE g_module_handle = NULL;
static bool g_has_closed = false;
static graphics_buffer g_front_buffer = {0}, g_back_buffer = {0};
const int kTimerID = 101;

graphics_buffer create_graphics_buffer(int wd, int hgt)
{
    HDC hdcScreen = GetDC(NULL);

    BITMAPINFO bmi = {0};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = wd;
    bmi.bmiHeader.biHeight = -hgt; // top-down
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    graphics_buffer gb;
    gb.hbm = CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, (void**)(&gb.data), NULL, NULL);

    ReleaseDC(NULL, hdcScreen);
    return gb;
}

bool window_manager_swap_buffer(void)
{
    //hBMP = (HBITMAP) LoadImage( NULL, "a.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}



LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hDC;
    RECT client;
    DWORD result;

    switch(message) 
    {
        case WM_CREATE:
            RECT r;
            GetClientRect(g_window_handle, &r);
            SetTimer(hWnd, kTimerID, 1, NULL);
            break;
        case WM_PAINT:
            hDC = BeginPaint(hwnd, &ps);
            GetClientRect(hwnd, &client);
            result = StretchDIBits(hDC,
                                   0, 0,
                                   client.right, client.bottom,
                                   0, 0,
                                   hBMP->bmWidth, hBMP->bmHeight,
                                   gBMP->bmBits, &m_bi, DIB_RGB_COLORS, SRCCOPY);
            if(result != g_framebuffer_height) printf("StretchDIBits failed! \n");
            EndPaint(hwnd, &ps);
            break;
        case WM_DESTROY:
            g_has_closed = true;
            PostQuitMessage(0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

bool window_manager_init(void)
{
    g_module_handle = GetModuleHandle(NULL);

    WNDCLASSEX wincl = {0};
    wincl.hInstance = g_module_handle;
    wincl.lpszClassName = g_window_class_name;
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof (WNDCLASSEX);
    wincl.hIcon = NULL;
    wincl.hIconSm = NULL;
    wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0; 
    wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

    if(!RegisterClassEx(&wincl))
        return false;

    g_window_handle = CreateWindowEx(
        0,                   /* Extended possibilites for variation */
        g_window_class_name,         /* Classname */
        g_window_title,       /* Title Text */
        WS_OVERLAPPEDWINDOW, /* default window */
        CW_USEDEFAULT,       /* Windows decides the position */
        CW_USEDEFAULT,       /* where the window ends up on the screen */
        g_framebuffer_width,                 /* The programs width */
        g_framebuffer_height,                 /* and height in pixels */
        HWND_DESKTOP,        /* The window is a child-window to desktop */
        NULL,                /* No menu */
        g_module_handle,       /* Program Instance handler */
        NULL                 /* No Window Creation data */
        );

    ShowWindow (g_window_handle, SW_SHOW);

    g_has_closed = false;

    return true;
}

bool window_manager_poll(void)
{
    MSG messages = {0};
    while(GetMessage(&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }
    return true;
}

bool window_manager_shutdown(void)
{
    //DestroyWindow(hwnd);
    if (g_framebuffer) free(g_framebuffer);
    if (g_backbuffer) free(g_backbuffer);
    return true;
}

bool window_manager_has_closed(void)
{
    return g_has_closed;
}