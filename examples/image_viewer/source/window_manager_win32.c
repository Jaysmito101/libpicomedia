
#include "window_manager.h"

#include <stdlib.h>
#include <stdio.h>

#include <Windows.h>

#define RGBA(r, g, b, a) ((b) | ((g) << 8) | ((r) << 16) | ((a) << 24))

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
static int g_framebuffer_width = 512, g_framebuffer_height = 512;
static graphics_buffer g_front_buffer = {0};
static unsigned int* g_back_buffer = NULL;

static graphics_buffer create_graphics_buffer(int wd, int hgt)
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
    gb.hbm = CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, (void**)(&gb.data), INVALID_HANDLE_VALUE, (DWORD)0);
    ReleaseDC(NULL, hdcScreen);
    return gb;
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message) 
    {
        case WM_CREATE:
            RECT r;
            GetClientRect(g_window_handle, &r);
            break;
        break;
        case WM_PAINT:
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            HDC hdc_bmp = CreateCompatibleDC(hdc);
            HBITMAP old_bmp = SelectObject(hdc_bmp, g_front_buffer.hbm);
            BitBlt(hdc, 0, 0, g_framebuffer_width, g_framebuffer_height, hdc_bmp, 0, 0, SRCCOPY);
            SelectObject(hdc, old_bmp);
            DeleteDC(hdc_bmp);
            EndPaint(hwnd, &ps);
            break;
        case WM_DESTROY:
            g_has_closed = true;
            break;
        case WM_CLOSE:
            g_has_closed = true;
            DestroyWindow(hwnd);
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}


bool window_manager_swap_buffer(void)
{
    memcpy(g_front_buffer.data, g_back_buffer, g_framebuffer_width * g_framebuffer_height * sizeof(unsigned int));
    InvalidateRect(g_window_handle, NULL, FALSE);
    return true;
}


bool window_manager_set_pixel(float x, float y, float r, float g, float b, float a)
{
    int wd = g_framebuffer_width, hgt = g_framebuffer_height;
    int px = (int)(x * (wd - 1)), py = (int)(y * (hgt - 1));
    if (px < 0 || px >= wd || py < 0 || py >= hgt) return false;
    int index = py * wd + px;
    int pvr = (int)(r * 255.0f), pvg = (int)(g * 255.0f), pvb = (int)(b * 255.0f), pva = (int)(a * 255.0f);
    g_back_buffer[index] = RGBA(pvr, pvg, pvb, pva);
    return true;
}

bool window_manager_clear(float r, float g, float b, float a)
{
    int wd = g_framebuffer_width, hgt = g_framebuffer_height;
    int pvr = (int)(r * 255.0f), pvg = (int)(g * 255.0f), pvb = (int)(b * 255.0f), pva = (int)(a * 255.0f);
    unsigned int pixel = RGBA(pvr, pvg, pvb, pva);
    for (int i = 0; i < wd * hgt; ++i) g_back_buffer[i] = pixel;
    return true;
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
    {
        printf("Failed to register window class! \n");
        return false;
    }

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

    g_front_buffer = create_graphics_buffer(g_framebuffer_width, g_framebuffer_height);
    g_back_buffer = malloc(g_framebuffer_width * g_framebuffer_height * sizeof(unsigned int));
    if (!g_back_buffer) 
    {
        printf("Failed to allocate back buffer! \n");
        return false;
    }

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
    DeleteObject(g_front_buffer.hbm);
    free(g_back_buffer);
    return true;
}

bool window_manager_has_closed(void)
{
    return g_has_closed;
}