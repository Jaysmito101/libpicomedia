#include "window_manager.h"

// NOTE: The wayland window manger has been written by Copilot, so its better to to trust it fully

#include <stdio.h>
#include <stdlib.h>

#include <wayland-client.h>
#include <wayland-egl.h>
#include <wayland-cursor.h>

#define RGBA(r, g, b, a) ((b) | ((g) << 8) | ((r) << 16) | ((a) << 24))

static int g_framebuffer_width = 512, g_framebuffer_height = 512;
static struct wl_display* g_display = NULL;
static struct wl_registry* g_registry = NULL;
static struct wl_compositor* g_compositor = NULL;
static struct wl_surface* g_surface = NULL;
static struct wl_shell* g_shell = NULL;
static struct wl_shell_surface* g_shell_surface = NULL;
static struct wl_egl_window* g_egl_window = NULL;
static bool g_has_closed = false;
static EGLDisplay g_egl_display = NULL;
static EGLContext g_egl_context = NULL;
static EGLSurface g_egl_surface = NULL;
static unsigned int* g_back_buffer_data = NULL;

bool window_manager_init(void)
{
    g_display = wl_display_connect(NULL);
    if (!g_display)
    {
        printf("Failed to open display! \n");
        return false;
    }

    g_registry = wl_display_get_registry(g_display);
    wl_registry_add_listener(g_registry, &registry_listener, NULL);
    wl_display_dispatch(g_display);
    wl_display_roundtrip(g_display);

    g_surface = wl_compositor_create_surface(g_compositor);
    g_shell_surface = wl_shell_get_shell_surface(g_shell, g_surface);
    wl_shell_surface_set_toplevel(g_shell_surface);

    g_egl_window = wl_egl_window_create(g_surface, g_framebuffer_width, g_framebuffer_height);

    g_egl_display = eglGetDisplay((EGLNativeDisplayType)g_display);
    if (g_egl_display == EGL_NO_DISPLAY)
    {
        printf("Failed to get EGL display! \n");
        return false;
    }

    EGLint major, minor;
    if (!eglInitialize(g_egl_display, &major, &minor))
    {
        printf("Failed to initialize EGL! \n");
        return false;
    }

    EGLint config_attribs[] =
    {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };

    EGLConfig config;
    EGLint num_configs;
    if (!eglChooseConfig(g_egl_display, config_attribs, &config, 1, &num_configs))
    {
        printf("Failed to choose EGL config! \n");
        return false;
    }

    EGLint context_attribs[] =
    {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    g_egl_context = eglCreateContext(g_egl_display, config, EGL_NO_CONTEXT, context_attribs);
    if (g_egl_context == EGL_NO_CONTEXT)
    {
        printf("Failed to create EGL context! \n");
        return false;
    }

    g_egl_surface = eglCreateWindowSurface(g_egl_display, config, g_egl_window, NULL);
    if (g_egl_surface == EGL_NO_SURFACE)
    {
        printf("Failed to create EGL surface! \n");
        return false;
    }

    if (!eglMakeCurrent(g_egl_display, g_egl_surface, g_egl_surface, g_egl_context))
    {
        printf("Failed to make EGL context current! \n");
        return false;
    }

    g_back_buffer_data = malloc(g_framebuffer_width * g_framebuffer_height * sizeof(unsigned int));
    if(!g_back_buffer_data) 
    {
        printf("Failed to allocate back buffer! \n");
        return false;
    }

    g_has_closed = false;

    return true;
}

bool window_manager_poll(void)
{
    wl_display_dispatch_pending(g_display);

    return true;
}


bool window_manager_has_closed(void)
{
    return g_has_closed;
}

bool window_manager_shutdown(void)
{
    eglMakeCurrent(g_egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroySurface(g_egl_display, g_egl_surface);
    eglDestroyContext(g_egl_display, g_egl_context);
    eglTerminate(g_egl_display);
    wl_egl_window_destroy(g_egl_window);
    wl_shell_surface_destroy(g_shell_surface);
    wl_surface_destroy(g_surface);
    wl_shell_destroy(g_shell);
    wl_compositor_destroy(g_compositor);
    wl_registry_destroy(g_registry);
    wl_display_disconnect(g_display);
    free(g_back_buffer_data);
    return true;
}

bool window_manager_swap_buffer(void)
{
    // Create a new buffer
    struct wl_buffer *buffer = wl_compositor_create_buffer(g_compositor, g_framebuffer_width, g_framebuffer_height, WL_SHM_FORMAT_ARGB8888);
    if (!buffer)
    {
        printf("Failed to create Wayland buffer! \n");
        return false;
    }

    // Get the data pointer of the new buffer
    void *data = wl_shm_buffer_get_data(buffer);

    // Copy the back buffer data to the new buffer
    memcpy(data, g_back_buffer_data, g_framebuffer_width * g_framebuffer_height * sizeof(unsigned int));

    // Attach the new buffer to the surface
    wl_surface_attach(g_surface, buffer, 0, 0);

    // Set the damage region to the entire surface
    wl_surface_damage(g_surface, 0, 0, g_framebuffer_width, g_framebuffer_height);

    // Commit the surface
    wl_surface_commit(g_surface);

    // Wait for the surface to be drawn
    wl_display_roundtrip(g_display);

    // Destroy the old buffer
    wl_buffer_destroy(buffer);

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
