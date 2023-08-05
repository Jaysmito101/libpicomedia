#include "libpicomedia/image/ppm/ppm.h"

// -----------------------------------------------------------------------------------------------

PM_Bool PM__ImagePPMWriteHeader(PM_Stream* stream, PM_Image* image, PM_UInt32 ppmFormat)
{
    // Write the magic number
    PM_StreamWriteInt8(stream, 'P');
    PM_StreamWriteInt8(stream, (PM_Int8)ppmFormat);
    PM_StreamWriteInt8(stream, '\n');

    if (ppmFormat == '3')
    {
        static const PM_Char* PPM_CREATOR_COMMENT = "# Created by libpicomedia";
        PM_StreamWrite(stream, PPM_CREATOR_COMMENT, strlen(PPM_CREATOR_COMMENT));
        PM_StreamWriteInt8(stream, '\n');
    }

    // Write the width and height
    static PM_Char widthHeightBuffer[128];
    sprintf(widthHeightBuffer, "%d %d", image->width, image->height);
    PM_StreamWrite(stream, widthHeightBuffer, strlen(widthHeightBuffer));
    PM_StreamWriteInt8(stream, '\n');

    // Write the max color value
    if (image->dataType == PICOIMEDIA_IMAGE_DATA_TYPE_UINT8)
    {
        static const PM_Char* PPM_MAX_COLOR_VALUE = "255";
        PM_StreamWrite(stream, PPM_MAX_COLOR_VALUE, strlen(PPM_MAX_COLOR_VALUE));
    }
    else if (image->dataType == PICOIMEDIA_IMAGE_DATA_TYPE_UINT16)
    {
        static const PM_Char* PPM_MAX_COLOR_VALUE = "65535";
        PM_StreamWrite(stream, PPM_MAX_COLOR_VALUE, strlen(PPM_MAX_COLOR_VALUE));
    }
    else
    {
        PM_LogError("Invalid PPM format! \n");
        return PM_FALSE;
    }
    PM_StreamWriteInt8(stream, '\n');

    return PM_TRUE;        
}


// -----------------------------------------------------------------------------------------------

PM_Bool PM__ImagePPMCheckOkForWrite(PM_Image* image)
{
    if (image->numChannels != 3)
    {
        PM_LogError("PPM only supports 3 channel images! \n");
        return PM_FALSE;
    }

    if (image->channelFormat != PICOMEDIA_IMAGE_CHANNEL_FORMAT_RGB)
    {
        PM_LogError("PPM only supports RGB channel format! \n");
        return PM_FALSE;
    }

    if (image->dataType != PICOIMEDIA_IMAGE_DATA_TYPE_UINT8 && image->dataType != PICOIMEDIA_IMAGE_DATA_TYPE_UINT16)
    {
        PM_LogError("PPM only supports UINT8 and UINT16 color values! \n");
        return PM_FALSE;
    }

    return PM_TRUE;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImagePPMWriteP6(PM_Stream* stream, PM_Image* image)
{

    if ( ! PM__ImagePPMCheckOkForWrite(image) )
    {
        return PM_FALSE;
    }

    PM_StreamSetCursorPosition(stream, 0);

    if ( ! PM__ImagePPMWriteHeader(stream, image, '6') )
    {
        PM_LogError("Failed to write PPM header! \n");
        return PM_FALSE;
    }
    
    if (! PM_StreamWrite(stream, image->data, image->dataSize) )
    {
        PM_LogError("Failed to write PPM data! \n");
        return PM_FALSE;
    }

    return PM_TRUE;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImagePPMWriteP3(PM_Stream* stream, PM_Image* image) 
{

    if ( ! PM__ImagePPMCheckOkForWrite(image) )
    {
        return PM_FALSE;
    }

    PM_StreamSetCursorPosition(stream, 0);

    if ( ! PM__ImagePPMWriteHeader(stream, image, '3') )
    {
        PM_LogError("Failed to write PPM header! \n");
        return PM_FALSE;
    }

    PM_Size bytesPerPixel  = (image->bitsPerChannel / 8) * image->numChannels;

    PM_Char pixelValueBuffer[128];
    for (PM_UInt32 i = 0; i < image->height; i++)
    {
        for (PM_UInt32 j = 0; j < image->width; j++)
        {
            PM_Size pixelIndex = (i * image->width) + j;
            PM_Size pixelOffset = pixelIndex * bytesPerPixel;

            if (image->dataType == PICOIMEDIA_IMAGE_DATA_TYPE_UINT8)
            {
                PM_UInt8* pixel = (PM_UInt8*)&image->data[pixelOffset];
                sprintf(pixelValueBuffer, "%d %d %d ", pixel[0], pixel[1], pixel[2]);
            }
            else if (image->dataType == PICOIMEDIA_IMAGE_DATA_TYPE_UINT16)
            {
                PM_UInt16* pixel = (PM_UInt16*)&image->data[pixelOffset];
                sprintf(pixelValueBuffer, "%d %d %d ", pixel[0], pixel[1], pixel[2]);
            }
            else
            {
                PM_LogError("Invalid PPM format! \n");
                return PM_FALSE;
            }

            PM_StreamWrite(stream, pixelValueBuffer, strlen(pixelValueBuffer));
        }
        PM_StreamWriteInt8(stream, '\n');
    }

    return PM_TRUE;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImagePPMWrite(PM_UInt32 ppmFormat, PM_Stream* stream, PM_Image* image)
{
    if (ppmFormat == PICOMEDIA_PPM_FORMAT_P3)
    {
        return PM_ImagePPMWriteP3(stream, image);
    }
    else if (ppmFormat == PICOMEDIA_PPM_FORMAT_P6)
    {
        return PM_ImagePPMWriteP6(stream, image);
    }
    else
    {
        PM_LogError("Invalid PPM format! \n");
        return PM_FALSE;
    }
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImagePPMWriteToFile(PM_UInt32 ppmFormat, const char* filePath, PM_Image* image)
{
    PM_Stream stream = {0};
    if ( ! PM_StreamInitFromFile(&stream, filePath, PICOMEDIA_STREAM_FLAG_WRITE) ) 
    {
        PM_LogError("Failed to initialize stream from file! \n");
        return PM_FALSE;
    }

    PM_Bool writeResult = PM_ImagePPMWrite(ppmFormat, &stream, image);

    PM_StreamDestroy(&stream);

    return writeResult;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImagePPMWriteToMemory(PM_UInt32 ppmFormat, PM_Image* image, PM_Byte* data, PM_Size* dataSize, PM_Size maxDataSize)
{
    PM_Stream stream = {0};
    if ( ! PM_StreamInitFromMemory(&stream, data, maxDataSize, PICOMEDIA_STREAM_FLAG_WRITE, false) ) 
    {
        PM_LogError("Failed to initialize stream from memory! \n");
        return PM_FALSE;
    }

    PM_Bool writeResult = PM_ImagePPMWrite(ppmFormat, &stream, image);

    if (writeResult)
    {
        *dataSize = PM_StreamGetCursorPosition(&stream);
    }

    PM_StreamDestroy(&stream);

    return writeResult;
}

// -----------------------------------------------------------------------------------------------