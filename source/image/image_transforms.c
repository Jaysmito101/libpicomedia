#include "libpicomedia/image/image_transforms.h"

// -----------------------------------------------------------------------------------------------

static void PM__ImageTransformsRGBToBGR(PM_Byte* srcPixel, PM_Byte* destPixel, PM_Size channelSize)
{
    PM_Memcpy(destPixel, srcPixel + channelSize * 2, channelSize);
    PM_Memcpy(destPixel + channelSize, srcPixel + channelSize, channelSize);
    PM_Memcpy(destPixel + channelSize * 2, srcPixel, channelSize);
}

// -----------------------------------------------------------------------------------------------

static void PM__ImageTransformsRGBAToBGRA(PM_Byte* srcPixel, PM_Byte* destPixel, PM_Size channelSize)
{
    PM_Memcpy(destPixel, srcPixel + channelSize * 2, channelSize);
    PM_Memcpy(destPixel + channelSize, srcPixel + channelSize, channelSize);
    PM_Memcpy(destPixel + channelSize * 2, srcPixel, channelSize);
    PM_Memcpy(destPixel + channelSize * 3, srcPixel + channelSize * 3, channelSize);
}

// -----------------------------------------------------------------------------------------------

static void PM__ImageTransformsRGBToRGBA(PM_Byte* srcPixel, PM_Byte* destPixel, PM_Size channelSize)
{
    PM_Memcpy(destPixel, srcPixel, channelSize);
    PM_Memcpy(destPixel + channelSize, srcPixel + channelSize, channelSize);
    PM_Memcpy(destPixel + channelSize * 2, srcPixel + channelSize * 2, channelSize);
    PM_Memset(destPixel + channelSize * 3, 255, channelSize);
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImageTransformsChangeChannelFormat(PM_Image* image, PM_UInt32 newChannelFormat)
{
    PM_Assert(image != NULL);

    if (image->channelFormat == newChannelFormat)
    {
        return PM_TRUE;
    }

    PM_UInt8 newNumChannels = 0;

    switch (newChannelFormat)
    {
    case PICOMEDIA_IMAGE_CHANNEL_FORMAT_RGB:
    case PICOMEDIA_IMAGE_CHANNEL_FORMAT_BGR:
        newNumChannels = 3;
        break;
    case PICOMEDIA_IMAGE_CHANNEL_FORMAT_RGBA:
    case PICOMEDIA_IMAGE_CHANNEL_FORMAT_BGRA:
        newNumChannels = 4;
        break;
    case PICOMEDIA_IMAGE_CHANNEL_FORMAT_GRAY:
        newNumChannels = 1;
        break;
    case PICOMEDIA_IMAGE_CHANNEL_FORMAT_GRAYA:
        newNumChannels = 2;
        break;
    }

    PM_Image newImage = {0};
    PM_ImageInit(&newImage);

    if(!PM_ImageAllocate(&newImage, image->width, image->height, newChannelFormat, image->dataType, newNumChannels))
    {
        PM_LogError("Failed to allocate memory for new image!");
        return PM_FALSE;
    }

    PM_Size channelSize = image->bitsPerChannel / 8;

    for (PM_UInt32 i = 0; i < image->height; i++)
    {
        for (PM_UInt32 j = 0; j < image->width; j++)
        {
            PM_Byte* pixelSrc = image->data + (i * image->width * image->numChannels) + (j * image->numChannels);
            PM_Byte* pixelDst = newImage.data + (i * newImage.width * newImage.numChannels) + (j * newImage.numChannels);

            // Both RGB to BGR and BGR to RGB are the same operation.
            if ( (image->channelFormat == PICOMEDIA_IMAGE_CHANNEL_FORMAT_RGB && newImage.channelFormat == PICOMEDIA_IMAGE_CHANNEL_FORMAT_BGR)
            || (image->channelFormat == PICOMEDIA_IMAGE_CHANNEL_FORMAT_BGR && newImage.channelFormat == PICOMEDIA_IMAGE_CHANNEL_FORMAT_RGB))
            {
                PM__ImageTransformsRGBToBGR(pixelSrc, pixelDst, channelSize);
            }
            else if ( (image->channelFormat == PICOMEDIA_IMAGE_CHANNEL_FORMAT_RGBA && newImage.channelFormat == PICOMEDIA_IMAGE_CHANNEL_FORMAT_BGRA)
            || (image->channelFormat == PICOMEDIA_IMAGE_CHANNEL_FORMAT_BGRA && newImage.channelFormat == PICOMEDIA_IMAGE_CHANNEL_FORMAT_RGBA))
            {
                PM__ImageTransformsRGBAToBGRA(pixelSrc, pixelDst, channelSize);
            }
            else if ((image->channelFormat == PICOMEDIA_IMAGE_CHANNEL_FORMAT_RGB && newImage.channelFormat == PICOMEDIA_IMAGE_CHANNEL_FORMAT_RGBA)
            || (image->channelFormat == PICOMEDIA_IMAGE_CHANNEL_FORMAT_BGR && newImage.channelFormat == PICOMEDIA_IMAGE_CHANNEL_FORMAT_BGRA))
            {
                PM__ImageTransformsRGBToRGBA(pixelSrc, pixelDst, channelSize);
            }
            else if ((image->channelFormat == PICOMEDIA_IMAGE_CHANNEL_FORMAT_RGBA && newImage.channelFormat == PICOMEDIA_IMAGE_CHANNEL_FORMAT_RGB)
            || (image->channelFormat == PICOMEDIA_IMAGE_CHANNEL_FORMAT_BGRA && newImage.channelFormat == PICOMEDIA_IMAGE_CHANNEL_FORMAT_BGR))
            {
                PM_Memcpy(pixelDst, pixelSrc, channelSize * 3);
            }
            else
            {
                PM_LogError("Unsupported channel format conversion!");
                PM_ImageDestroy(&newImage);
                return PM_FALSE;
            }

        }
    }


    if(! PM_ImageCopy(image, &newImage))
    {
        PM_LogError("Failed to copy image!");
        PM_ImageDestroy(&newImage);
        return PM_FALSE;
    }

    PM_ImageDestroy(&newImage);

    return PM_TRUE;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImageTransformsChangeDataType(PM_Image* image, PM_UInt32 newDataType)
{
    PM_Assert(image != NULL);

    if (image->dataType == newDataType)
    {
        return PM_TRUE;
    }

    PM_Image newImage = {0};
    PM_ImageInit(&newImage);

    if(!PM_ImageAllocate(&newImage, image->width, image->height, image->channelFormat, newDataType, image->numChannels))
    {
        PM_LogError("Failed to allocate memory for new image!");
        return PM_FALSE;
    }

    for (PM_UInt32 i = 0; i < image->height; i++)
    {
        for (PM_UInt32 j = 0; j < image->width; j++)
        {
            for (PM_UInt8 k = 0; k < image->numChannels; k++)
            {
                PM_ImageSetPixelValue(&newImage, j, i, k, PM_ImageGetPixelValue(image, j, i, k, NULL));
            }
        }
    }

    if (! PM_ImageCopy(image, &newImage))
    {
        PM_LogError("Failed to copy image!");
        PM_ImageDestroy(&newImage);
        return PM_FALSE;
    }

    PM_ImageDestroy(&newImage);

    return PM_TRUE;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImageTransformsFlipHorizontal(PM_Image* image)
{
    PM_Assert(image != NULL);

    PM_Size pixelSize = image->bitsPerChannel / 8 * image->numChannels;
    static PM_Byte pixelBuffer[1024] = {0};

    for (PM_UInt32 i = 0; i < image->height; i++)
    {
        PM_Byte* row = image->data + (i * image->width * pixelSize);

        for (PM_UInt32 j = 0; j < image->width / 2; j++)
        {
            PM_Byte* pixel = row + (j * pixelSize);
            PM_Byte* pixel2 = row + ((image->width - j - 1) * pixelSize);

            PM_Memcpy(pixelBuffer, pixel, pixelSize);
            PM_Memcpy(pixel, pixel2, pixelSize);
            PM_Memcpy(pixel2, pixelBuffer, pixelSize);
            
        }
    }

    return PM_TRUE;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImageTransformsFlipVertical(PM_Image* image)
{
    PM_Assert(image != NULL);

    PM_Size pixelSize = image->bitsPerChannel / 8 * image->numChannels;
    PM_Size rowSize = pixelSize * image->width;
    PM_Byte* rowBuffer = (PM_Byte*)PM_Malloc(rowSize);
    
    if (rowBuffer == NULL)
    {
        PM_LogError("Failed to allocate memory for row buffer!");
        return PM_FALSE;
    }

    for (PM_UInt32 i = 0; i < image->height / 2; i++)
    {
        PM_Byte* row = image->data + (i * rowSize);
        PM_Byte* row2 = image->data + ((image->height - i - 1) * rowSize);

        PM_Memcpy(rowBuffer, row, rowSize);
        PM_Memcpy(row, row2, rowSize);
        PM_Memcpy(row2, rowBuffer, rowSize);
    }

    PM_Free(rowBuffer);
    return PM_TRUE;
}

// -----------------------------------------------------------------------------------------------
