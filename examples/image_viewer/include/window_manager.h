#ifndef LPM_IMGV_WINDOW_MANAGER_H
#define LPM_IMGV_WINDOW_MANAGER_H

#include <stdbool.h>

bool window_manager_init(void);
bool window_manager_poll(void);
bool window_manager_has_closed(void);
bool window_manager_shutdown(void);
bool window_manager_swap_buffer(void);
bool window_manager_set_pixel(float x, float y, float r, float g, float b, float a);
bool window_manager_clear(float r, float g, float b, float a);

#endif
