#include "libpicomedia/image/image_base.h"

void PM_ImageInit(PM_Image* image)
{
    image->width = 0;
    image->height = 0;
    image->data = NULL;
    image->dataCapacity = 0;
    image->numChannels = 0;
    image->bitsPerChannel = 0;
    image->channelFormat = PICOMEDIA_IMAGE_CHANNEL_FORMAT_UNKNOWN;
    image->dataType = PICOIMEDIA_IMAGE_DATA_TYPE_UNKNOWN;
}

void PM_ImageDestroy(PM_Image* image)
{
    if (image->data != NULL)
    {
        PM_Free(image->data);
        image->data = NULL;
    }
    PM_ImageInit(image);
}
