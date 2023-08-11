#include "libpicomedia/image/ppm/ppm.h"

// -----------------------------------------------------------------------------------------------

// Skips all whitespace and comments
static PM_Bool PM__ImagePPMSkipASCII(PM_Stream* stream)
{
    PM_Bool isComment = PM_FALSE;
    while (PM_TRUE)
    {
        PM_Char ch = PM_StreamReadInt8(stream);
        if (ch == 0) // EOF
        {
            return PM_FALSE;
        }
        if (!isComment)
        {
            if (ch == '#')
            {
                isComment = PM_TRUE;
            }
            else if (PM_CharIsWhiteSpace(ch))
            {
                continue;
            }
            else
            {
                PM_StreamSetCursorPosition(stream, PM_StreamGetCursorPosition(stream) - 1);
                break;
            }
        }
        else if (ch == '\n')
        {
            isComment = PM_FALSE;
        }
    }
    return PM_TRUE;
}

// -----------------------------------------------------------------------------------------------

static PM_Bool PM__ImagePPMReadHeader(PM_Stream* stream, PM_Image* image, PM_UInt8 magicNumber, PM_UInt32* maxColorValue)
{
    if ( (PM_StreamReadInt8(stream) != 'P') || (PM_StreamReadInt8(stream) != magicNumber) )
    {
        PM_LogError("Provided Image is not PPM.P%c", magicNumber);
        return PM_FALSE;
    }
    
    // Skip the whitespace and comments
    PM__ImagePPMSkipASCII(stream); 

    // Read the width
    image->width = (PM_UInt32)PM_ReadASCIIIntegerFromStream(stream);

    // Skip the whitespace and comments
    PM__ImagePPMSkipASCII(stream);

    // Read the height
    image->height = (PM_UInt32)PM_ReadASCIIIntegerFromStream(stream);

    // Skip the whitespace and comments
    PM__ImagePPMSkipASCII(stream);
    
    // Read max color value
    *maxColorValue = (PM_UInt32)PM_ReadASCIIIntegerFromStream(stream);
    *maxColorValue = max(*maxColorValue, 0);
    *maxColorValue = min(*maxColorValue, 65536);

    // PPM only supports 8-bit and 16-bit color values
    if ( *maxColorValue > 255 )
    {
        image->dataType = PICOIMEDIA_IMAGE_DATA_TYPE_UINT16;
    }
    else
    {
        image->dataType = PICOIMEDIA_IMAGE_DATA_TYPE_UINT8;
    }

    // PPM only supports RGB color format
    image->channelFormat = PICOMEDIA_IMAGE_CHANNEL_FORMAT_RGB;
    image->numChannels = 3;

    // Skip the whitespace
    PM__ImagePPMSkipASCII(stream);

    return PM_TRUE;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImagePPMReadP6(PM_Stream* stream, PM_Image* image)
{
    PM_Assert(stream != NULL);
    PM_Assert(image != NULL);

    PM_UInt32 maxColorValue = 0;

    PM_StreamSetCursorPosition(stream, 0);

    if ( ! PM__ImagePPMReadHeader(stream, image, '6', &maxColorValue) )
    {
        PM_LogError("Failed to read PPM header! \n");
        return PM_FALSE;
    }

    // Allocate memory for the image
    if (! PM_ImageAllocate(image, image->width, image->height, image->channelFormat, image->dataType, 3))
    {
        PM_LogError("Failed to allocate memory for the image! \n");
        return PM_FALSE;
    }

    // Read the image data
    PM_Size bytesPerChannel = image->bitsPerChannel / 8;
    PM_Size bytesPerPixel = bytesPerChannel * image->numChannels;
    PM_Size rowSize = image->width * bytesPerPixel;
    PM_Size pixelOffset = 0;

    PM_Byte* rowData = (PM_Byte*)PM_Malloc(rowSize);
    if (rowData == NULL)
    {
        PM_LogError("Failed to allocate memory for the image row data! \n");
        return PM_FALSE;
    }

    // print the header info

    for (PM_UInt32 y = 0; y < image->height; y++)
    {
        if (PM_StreamRead(stream, rowData, rowSize) != rowSize)
        {
            PM_LogError("Failed to read image data! \n");
            PM_Free(rowData);
            return PM_FALSE;
        }

        for (PM_UInt32 x = 0 ; x < image->width ; x++)
        {
            pixelOffset = (y * image->width + x) * bytesPerPixel;
            if (image->dataType == PICOIMEDIA_IMAGE_DATA_TYPE_UINT8)
            {
                *((PM_UInt8*)(image->data + pixelOffset + 0 * bytesPerChannel)) = (PM_UInt8)( ((PM_Float64)rowData[x * bytesPerPixel + 0] / (PM_Float64)maxColorValue) * 255.0);
                *((PM_UInt8*)(image->data + pixelOffset + 1 * bytesPerChannel)) = (PM_UInt8)( ((PM_Float64)rowData[x * bytesPerPixel + 1] / (PM_Float64)maxColorValue) * 255.0);
                *((PM_UInt8*)(image->data + pixelOffset + 2 * bytesPerChannel)) = (PM_UInt8)( ((PM_Float64)rowData[x * bytesPerPixel + 2] / (PM_Float64)maxColorValue) * 255.0);
            }
            else if (image->dataType == PICOIMEDIA_IMAGE_DATA_TYPE_UINT16)
            {
                *((PM_UInt16*)(image->data + pixelOffset + 0 * bytesPerChannel)) = (PM_UInt16)(((PM_Float64)rowData[x * bytesPerPixel + 0] / (PM_Float64)maxColorValue) * 65535.0);
                *((PM_UInt16*)(image->data + pixelOffset + 1 * bytesPerChannel)) = (PM_UInt16)(((PM_Float64)rowData[x * bytesPerPixel + 1] / (PM_Float64)maxColorValue) * 65535.0);
                *((PM_UInt16*)(image->data + pixelOffset + 2 * bytesPerChannel)) = (PM_UInt16)(((PM_Float64)rowData[x * bytesPerPixel + 2] / (PM_Float64)maxColorValue) * 65535.0);
            }
        }
    }

    return PM_TRUE;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImagePPMReadP3(PM_Stream* stream, PM_Image* image)
{
    PM_Assert(stream != NULL);
    PM_Assert(image != NULL);

    PM_UInt32 maxColorValue = 0;

    PM_StreamSetCursorPosition(stream, 0);

    if ( ! PM__ImagePPMReadHeader(stream, image, '3', &maxColorValue) )
    {
        PM_LogError("Failed to read PPM header! \n");
        return PM_FALSE;
    }

    // Allocate memory for the image
    if (! PM_ImageAllocate(image, image->width, image->height, image->channelFormat, image->dataType, 3))
    {
        PM_LogError("Failed to allocate memory for the image! \n");
        return PM_FALSE;
    }

    // Read the image
    PM_Size bytesPerChannel = image->bitsPerChannel / 8;
    PM_Size bytesPerPixel = bytesPerChannel * image->numChannels;
    PM_Byte* data = (PM_Byte*)image->data;

    for (PM_UInt32 y = 0; y < image->height; y++)
    {
        for (PM_UInt32 x = 0 ; x < image->width ; x++)
        {
            for (PM_UInt32 c = 0; c < image->numChannels; c++)
            {
                PM_Size offset = image->width * y * bytesPerPixel + x * bytesPerPixel + c * bytesPerChannel;

                if (!PM__ImagePPMSkipASCII(stream))
                {
                    PM_LogError("Failed to read the image data! \n");
                    return PM_FALSE;
                }

                PM_Int64 value = PM_ReadASCIIIntegerFromStream(stream);

                if (value > maxColorValue)
                {
                    PM_LogError("Invalid color value! \n");
                    return PM_FALSE;
                }

                if (image->dataType == PICOIMEDIA_IMAGE_DATA_TYPE_UINT8)
                {
                    *((PM_UInt8*)data + offset) = (PM_UInt8)value;
                }
                else if (image->dataType == PICOIMEDIA_IMAGE_DATA_TYPE_UINT16)
                {
                    *((PM_UInt16*)data + offset) = (PM_UInt16)value;
                }
                else 
                {
                    PM_LogError("Unsupported data type! \n");
                    return PM_FALSE;
                }
            }
        }
    }

    return PM_TRUE;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PICOMEDIA_API PM_ImagePPMRead(PM_Stream* stream, PM_Image* image)
{
    PM_Assert(stream != NULL);
    PM_Assert(image != NULL);

    PM_UInt32 ppmType = PM_ImagePPMDetect(stream);

    if (ppmType == PICOMEDIA_PPM_FORMAT_P3)
    {
        return PM_ImagePPMReadP3(stream, image);
    }
    else if (ppmType == PICOMEDIA_PPM_FORMAT_P6)
    {
        return PM_ImagePPMReadP6(stream, image);
    }
    else
    {
        PM_LogError("Failed to detect PPM type! \n");
        return PM_FALSE;
    }
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImagePPMReadFromMemory(PM_Byte* data, PM_Size dataSize, PM_Image* image)
{
    PM_Assert(data != NULL);
    PM_Assert(image != NULL);
    PM_Assert(dataSize > 0);

    PM_Stream stream = {0};
    if ( ! PM_StreamInitFromMemory(&stream, data, dataSize, PICOMEDIA_STREAM_FLAG_READ, false) ) 
    {
        PM_LogError("Failed to initialize stream from memory! \n");
        return PM_FALSE;
    }

    PM_Bool readResult = PM_ImagePPMRead(&stream, image);

    PM_StreamDestroy(&stream);

    return readResult;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImagePPMReadFromFile(const char* filePath, PM_Image* image)
{
    PM_Assert(filePath != NULL);
    PM_Assert(image != NULL);
    
    PM_Stream stream = {0};
    if ( ! PM_StreamInitFromFile(&stream, filePath, PICOMEDIA_STREAM_FLAG_READ) ) 
    {
        PM_LogError("Failed to initialize stream from file! \n");
        return PM_FALSE;
    }

    PM_Bool readResult = PM_ImagePPMRead(&stream, image);

    PM_StreamDestroy(&stream);

    return readResult;
}

// -----------------------------------------------------------------------------------------------
