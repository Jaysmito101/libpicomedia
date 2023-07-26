#include "window_manager.h"

#include <stdio.h>


int main()
{
    printf("Hello World! [Example] \n");

    if(!window_manager_init())
    {
        printf("Failed to initialize window manager! \n");
        return 1;
    }

    while (!window_manager_has_closed())
    {

        window_manager_clear(0.2f, 0.2f, 0.2f, 1.0f);

        for (int y = 200; y < 300; y++) for (int x = 200; x < 300; x++) window_manager_set_pixel(x/512.0f, y/512.0f, 1.0f, 0.0f, 0.0f, 1.0f);

        window_manager_swap_buffer();
        window_manager_poll();
    }

    if(!window_manager_shutdown())
    {
        printf("Failed to shutdown window manager! \n");
        return 1;
    }

    return 0;
}