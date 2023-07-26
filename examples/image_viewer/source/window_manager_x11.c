#include "window_manager.h"

#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/X.h>


#define RGBA(r, g, b, a) ((b) | ((g) << 8) | ((r) << 16) | ((a) << 24))

static int g_framebuffer_width = 512, g_framebuffer_height = 512;
static Display* g_display = NULL;
static int g_screen = 0;
static Window g_root_window = 0, g_window_handle = 0;
static bool g_has_closed = false;
static GC g_gc = 0;
static Pixmap g_front_buffer = 0;
static XImage* g_back_buffer = NULL;
static unsigned int* g_back_buffer_data = NULL;

bool window_manager_init(void)
{
    g_display = XOpenDisplay(NULL);
    if (!g_display)
    {
        printf("Failed to open display! \n");
        return false;
    }
    g_screen = DefaultScreen(g_display);
    g_root_window = RootWindow(g_display, g_screen);
 
    XSetWindowAttributes window_attributes = {0};

    window_attributes.background_pixel = WhitePixel(g_display, g_screen);
    window_attributes.border_pixel = BlackPixel(g_display, g_screen);
    window_attributes.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | PointerMotionMask | StructureNotifyMask;

 
    g_window_handle = XCreateWindow(g_display, g_root_window, 100, 100, g_framebuffer_width, g_framebuffer_height, 1, DefaultDepth(g_display, g_screen), InputOutput, DefaultVisual(g_display, g_screen), CWBackPixel | CWBorderPixel | CWEventMask, &window_attributes);
    XStoreName(g_display, g_window_handle, "libpicomedia: Image Viewer");
    XMapWindow(g_display, g_window_handle);

    XGCValues gc_values = {0};
    gc_values.foreground = BlackPixel(g_display, g_screen);
    gc_values.background = WhitePixel(g_display, g_screen);
    gc_values.line_style = LineSolid;
    gc_values.line_width = 1;
    gc_values.cap_style = CapButt;
    gc_values.join_style = JoinMiter;
    gc_values.fill_style = FillSolid;

    unsigned long gc_mask = GCBackground | GCForeground | GCLineStyle | GCLineWidth | GCCapStyle | GCJoinStyle | GCFillStyle;
    g_gc = XCreateGC(g_display, g_window_handle, gc_mask, &gc_values);

    g_front_buffer = XCreatePixmap(g_display, g_window_handle, g_framebuffer_width, g_framebuffer_height, DefaultDepth(g_display, g_screen));

    g_back_buffer_data = malloc(g_framebuffer_width * g_framebuffer_height * sizeof(unsigned int));
    if(!g_back_buffer_data) 
    {
        printf("Failed to allocate back buffer! \n");
        return false;
    }

    g_back_buffer = XCreateImage(g_display, DefaultVisual(g_display, g_screen), DefaultDepth(g_display, g_screen), ZPixmap, 0, (char*)g_back_buffer_data, g_framebuffer_width, g_framebuffer_height, 32, 0);
    if (!g_back_buffer)
    {
        printf("Failed to create back buffer! \n");
        return false;
    }



    g_has_closed = false;

    return true;
}

bool window_manager_poll(void)
{
    XEvent event = {0};

    while (XPending(g_display))
    {
        XNextEvent(g_display, &event);
        switch (event.type)
        {
        case Expose:
            break;
        case ConfigureNotify:
            break;
        case KeyPress:
            break;
        case ButtonPress:
            break;
        case MotionNotify:
            break;
        case ClientMessage:
            break;
        case DestroyNotify:
            g_has_closed = true;
            break;
        default:
            break;
        }
    }

    return true;
}


bool window_manager_has_closed(void)
{
    return g_has_closed;
}

bool window_manager_shutdown(void)
{
    XUnmapWindow(g_display, g_window_handle);
    XDestroyWindow(g_display, g_window_handle);
    XFreeGC(g_display, g_gc);
    XFreePixmap(g_display, g_front_buffer);
    XDestroyImage(g_back_buffer);
    free(g_back_buffer_data);
    XCloseDisplay(g_display);
    return true;
}

bool window_manager_swap_buffer(void)
{
    XPutImage(g_display, g_front_buffer, g_gc, g_back_buffer, 0, 0, 0, 0, g_framebuffer_width, g_framebuffer_height);
    XCopyArea(g_display, g_front_buffer, g_window_handle, g_gc, 0, 0, g_framebuffer_width, g_framebuffer_height, 0, 0);
    return true;
}

bool window_manager_set_pixel(float x, float y, float r, float g, float b, float a)
{
    int wd = g_framebuffer_width, hgt = g_framebuffer_height;
    int px = (int)(x * (wd - 1)), py = (int)(y * (hgt - 1));
    if (px < 0 || px >= wd || py < 0 || py >= hgt) return false;
    int index = py * wd + px;
    int pvr = (int)(r * 255.0f), pvg = (int)(g * 255.0f), pvb = (int)(b * 255.0f), pva = (int)(a * 255.0f);
    g_back_buffer_data[index] = RGBA(pvr, pvg, pvb, pva);
    return true;
}

bool window_manager_clear(float r, float g, float b, float a)
{
    int wd = g_framebuffer_width, hgt = g_framebuffer_height;
    int pvr = (int)(r * 255.0f), pvg = (int)(g * 255.0f), pvb = (int)(b * 255.0f), pva = (int)(a * 255.0f);
    unsigned int pixel = RGBA(pvr, pvg, pvb, pva);
    for (int i = 0; i < wd * hgt; ++i) g_back_buffer_data[i] = pixel;
    return true;
}


