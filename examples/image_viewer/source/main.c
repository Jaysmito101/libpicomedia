#include "window_manager.h"

#include <stdio.h>

#include "libpicomedia/libpicomedia.h"


int main()
{
    PM_LogInfo("Attempting to read image from file: %s", "test.png");

    PM_Image image = {0};

    if(!PM_ImagePNGDetectFromFile("test.png")) {
        printf("Failed to detect image type! \n");
        return 1;
    }

    if(!PM_ImagePNGReadFromFile("test.png", &image)) {
        printf("Failed to read image from file! \n");
        return 1;
    }


    return 0;
 
    if(!window_manager_init())
    {
        printf("Failed to initialize window manager! \n");
        return 1;
    }


    while (!window_manager_has_closed())
    {
        window_manager_clear(0.2f, 0.2f, 0.2f, 1.0f);

        for (int  i = 0; i < 512 ; i++)
        {
            for (int j = 0; j < 512; j++)
            {
                int j2 = (int)((j / 512.0f) * image.width);
                int i2 = (int)((i / 512.0f) * image.height);
                window_manager_set_pixel(j / 512.0f, i / 512.0f, (float)PM_ImageGetPixelValue(&image, j2, i2, 0, NULL), (float)PM_ImageGetPixelValue(&image, j2, i2, 1, NULL), (float)PM_ImageGetPixelValue(&image, j2, i2, 2, NULL), 1.0f);
            }
        }

        window_manager_swap_buffer();
        window_manager_poll();
    }

    if(!window_manager_shutdown())
    {
        printf("Failed to shutdown window manager! \n");
        return 1;
    }

    PM_ImageDestroy(&image);

    return 0;
}
