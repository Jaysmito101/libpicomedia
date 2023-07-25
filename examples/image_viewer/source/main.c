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