#include "window_manager.h"

#include <stdio.h>

#include "libpicomedia/libpicomedia.h"


int main()
{
    PM_LogInfo("Attempting to read image from file: %s", "test.ppm");

    PM_Image image = {0};
    if(!PM_ImagePPMReadFromFile("testp3.ppm", &image))
    {
        PM_LogInfo("Failed to read image from file: %s \n", "test.ppm");
        return 1;
    }

    if(!window_manager_init())
    {
        printf("Failed to initialize window manager! \n");
        return 1;
    }

    PM_ImagePPMWriteToFile(PICOMEDIA_PPM_FORMAT_P6, "testp6.ppm", &image);
    PM_ImagePPMWriteToFile(PICOMEDIA_PPM_FORMAT_P3, "testp3.ppm", &image);

    while (!window_manager_has_closed())
    {
        window_manager_clear(0.2f, 0.2f, 0.2f, 1.0f);

        for (int  i = 0; i < (int)image.height ; i++)
        {
            for (int j = 0; j < (int)image.width; j++)
            {
                window_manager_set_pixel(j / (float)image.height, i / (float)image.width, (float)PM_ImageGetPixelValue(&image, j, i, 0, NULL), (float)PM_ImageGetPixelValue(&image, j, i, 1, NULL), (float)PM_ImageGetPixelValue(&image, j, i, 2, NULL), 1.0f);
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