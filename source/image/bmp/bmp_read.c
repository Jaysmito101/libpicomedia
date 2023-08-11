#include "libpicomedia/image/bmp/bmp.h"

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImageBMPReadHeader(PM_Stream* stream, PM_BMPHeader* header)
{
    PM_Assert(stream != NULL);
    PM_Assert(header != NULL);

    PM_ImageBMPHeaderInit(header);

    PM_StreamSetCursorPosition(stream, 0);

    PM_StreamSetRequireReverse(stream, PM_IsBigEndian());

    if(PM_StreamReadInt8(stream) != 'B' || PM_StreamReadInt8(stream) != 'M')
    {
        PM_LogError("PM_ImageBMPReadHeader: Invalid BMP file signature.");
        return PM_FALSE;
    }

    ((PM_Byte*)&header->signature)[0] = 'B';
    ((PM_Byte*)&header->signature)[1] = 'M';

    if(PM_StreamRead(stream, (PM_Byte*)&header->fileSize, sizeof(PM_UInt32)) != sizeof(PM_UInt32))
    {
        PM_LogError("PM_ImageBMPReadHeader: Failed to read file size.");
        return PM_FALSE;
    }

    if(PM_StreamRead(stream, (PM_Byte*)&header->reserved, sizeof(PM_UInt32)) != sizeof(PM_UInt32))
    {
        PM_LogError("PM_ImageBMPReadHeader: Failed to read reserved field.");
        return PM_FALSE;
    }

    if(PM_StreamRead(stream, (PM_Byte*)&header->dataOffset, sizeof(PM_UInt32)) != sizeof(PM_UInt32))
    {
        PM_LogError("PM_ImageBMPReadHeader: Failed to read data offset.");
        return PM_FALSE;
    }

    return PM_TRUE;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImageBMPReadInfoHeader(PM_Stream* stream, PM_BMPInfoHeader* infoHeader)
{
    PM_Assert(stream != NULL);
    PM_Assert(infoHeader != NULL);

    PM_ImageBMPInfoHeaderInit(infoHeader);

    PM_StreamSetCursorPosition(stream, 0x000E); // Skip header size (14 bytes)

    PM_StreamSetRequireReverse(stream, PM_IsBigEndian());

    if(PM_StreamRead(stream, (PM_Byte*)&infoHeader->headerSize, sizeof(PM_UInt32)) != sizeof(PM_UInt32))
    {
        PM_LogError("PM_ImageBMPReadInfoHeader: Failed to read header size.");
        return PM_FALSE;
    }

    if(PM_StreamRead(stream, (PM_Byte*)&infoHeader->width, sizeof(PM_Int32)) != sizeof(PM_Int32))
    {
        PM_LogError("PM_ImageBMPReadInfoHeader: Failed to read image width.");
        return PM_FALSE;
    }

    if(PM_StreamRead(stream, (PM_Byte*)&infoHeader->height, sizeof(PM_Int32)) != sizeof(PM_Int32))
    {
        PM_LogError("PM_ImageBMPReadInfoHeader: Failed to read image height.");
        return PM_FALSE;
    }

    if(PM_StreamRead(stream, (PM_Byte*)&infoHeader->planes, sizeof(PM_UInt16)) != sizeof(PM_UInt16))
    {
        PM_LogError("PM_ImageBMPReadInfoHeader: Failed to read planes.");
        return PM_FALSE;
    }

    if (infoHeader->planes != 1)
    {
        PM_LogError("PM_ImageBMPReadInfoHeader: Invalid planes value(%d).", infoHeader->planes);
        return PM_FALSE;
    }

    if(PM_StreamRead(stream, (PM_Byte*)&infoHeader->bitsPerPixel, sizeof(PM_UInt16)) != sizeof(PM_UInt16))
    {
        PM_LogError("PM_ImageBMPReadInfoHeader: Failed to read bits per pixel.");
        return PM_FALSE;
    }

    if (infoHeader->bitsPerPixel != 1 && infoHeader->bitsPerPixel != 4 && infoHeader->bitsPerPixel != 8 && infoHeader->bitsPerPixel != 16 && infoHeader->bitsPerPixel != 24)
    {
        PM_LogError("PM_ImageBMPReadInfoHeader: Invalid bits per pixel value(%d).", infoHeader->bitsPerPixel);
        return PM_FALSE;
    }

    if(PM_StreamRead(stream, (PM_Byte*)&infoHeader->compression, sizeof(PM_UInt32)) != sizeof(PM_UInt32))
    {
        PM_LogError("PM_ImageBMPReadInfoHeader: Failed to read compression.");
        return PM_FALSE;
    }

    if (infoHeader->compression != 0 && infoHeader->compression != 1 && infoHeader->compression != 2)
    {
        PM_LogError("PM_ImageBMPReadInfoHeader: Invalid compression value(%d).", infoHeader->compression);
        return PM_FALSE;
    }

    if(PM_StreamRead(stream, (PM_Byte*)&infoHeader->imageSize, sizeof(PM_UInt32)) != sizeof(PM_UInt32))
    {
        PM_LogError("PM_ImageBMPReadInfoHeader: Failed to read image size.");
        return PM_FALSE;
    }

    if (infoHeader->imageSize == 0 && infoHeader->compression != 0)
    {
        PM_LogError("PM_ImageBMPReadInfoHeader: Invalid image size value(0 when compress is non zero).");
        return PM_FALSE;
    }

    if(PM_StreamRead(stream, (PM_Byte*)&infoHeader->xPixelsPerMeter, sizeof(PM_Int32)) != sizeof(PM_Int32))
    {
        PM_LogError("PM_ImageBMPReadInfoHeader: Failed to read horizontal resolution.");
        return PM_FALSE;
    }

    if(PM_StreamRead(stream, (PM_Byte*)&infoHeader->yPixelsPerMeter, sizeof(PM_Int32)) != sizeof(PM_Int32))
    {
        PM_LogError("PM_ImageBMPReadInfoHeader: Failed to read vertical resolution.");
        return PM_FALSE;
    }

    if(PM_StreamRead(stream, (PM_Byte*)&infoHeader->colorsUsed, sizeof(PM_UInt32)) != sizeof(PM_UInt32))
    {
        PM_LogError("PM_ImageBMPReadInfoHeader: Failed to read colors used.");
        return PM_FALSE;
    }

    if(PM_StreamRead(stream, (PM_Byte*)&infoHeader->colorsImportant, sizeof(PM_UInt32)) != sizeof(PM_UInt32))
    {
        PM_LogError("PM_ImageBMPReadInfoHeader: Failed to read important colors.");
        return PM_FALSE;
    }

    return PM_TRUE;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImageBMPReadColorTable(PM_Stream* stream, const PM_BMPInfoHeader* infoHeader, PM_BMPColorTableItem** colorTable, PM_Size* colorTableCapacity)
{
    PM_Assert(stream != NULL);
    PM_Assert(infoHeader != NULL);
    PM_Assert(colorTable != NULL);
    PM_Assert(colorTableCapacity != NULL);

    *colorTable = NULL;
    *colorTableCapacity = 0;

    if(infoHeader->colorsUsed == 0 || infoHeader->bitsPerPixel > 8)
    {
        return PM_TRUE;
    }

    if(infoHeader->colorsUsed > 256)
    {
        PM_LogError("PM_ImageBMPReadColorTable: Invalid colors used value(%d).", infoHeader->colorsUsed);
        return PM_FALSE;
    }

    *colorTableCapacity = infoHeader->colorsUsed;

    *colorTable = (PM_BMPColorTableItem*)PM_Malloc(sizeof(PM_BMPColorTableItem) * (*colorTableCapacity));

    if(*colorTable == NULL)
    {
        PM_LogError("PM_ImageBMPReadColorTable: Failed to allocate color table.");
        return PM_FALSE;
    }

    PM_StreamSetCursorPosition(stream, 0x0036); // 0x0036 is the offset of the color table in the file

    if(PM_StreamRead(stream, (PM_Byte*)(*colorTable), sizeof(PM_BMPColorTableItem) * (*colorTableCapacity)) != sizeof(PM_BMPColorTableItem) * (*colorTableCapacity))
    {
        PM_LogError("PM_ImageBMPReadColorTable: Failed to read color table.");
        return PM_FALSE;
    }

    return PM_TRUE;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImageBMPReadImageData(PM_Stream* stream, const PM_BMPHeader* header, PM_Byte** imageData, PM_Size* imageDataSize)
{
    PM_Assert(stream != NULL);
    PM_Assert(header != NULL);
    PM_Assert(imageData != NULL);
    PM_Assert(imageDataSize != NULL);

    *imageData = NULL;
    *imageDataSize = 0;

    *imageDataSize = header->fileSize - header->dataOffset;

    *imageData = (PM_Byte*)PM_Malloc(*imageDataSize);

    if (*imageData == NULL)
    {
        PM_LogError("PM_ImageBMPReadImageData: Failed to allocate image data.");
        return PM_FALSE;
    }

    PM_StreamSetCursorPosition(stream, header->dataOffset);

    if(PM_StreamRead(stream, *imageData, *imageDataSize) != *imageDataSize)
    {
        PM_LogError("PM_ImageBMPReadImageData: Failed to read image data.");
        return PM_FALSE;
    }

    return PM_TRUE;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImageBMPDecode(const PM_BMPContext* context, PM_Image* image)
{
    PM_Assert(context != NULL);
    PM_Assert(image != NULL);
    PM_Assert(image->data != NULL);
    PM_Assert(image->dataSize > 0);
    PM_Assert(context->imageData != NULL);

    if (context->infoHeader.bitsPerPixel <= 8)
    {
        PM_Size divBits = 8 / context->infoHeader.bitsPerPixel;
        PM_Size scanLineSize = (context->infoHeader.width) / divBits + ((context->infoHeader.width % divBits) ? 1 : 0);
        scanLineSize = (scanLineSize + 3) & ~3; // align to 4 bytes
        
        for (PM_Int64 y = image->height - 1 ; y >= 0 ; y --)
        {
            PM_Byte* scanLine = context->imageData + y * scanLineSize;
            PM_Byte* dst = image->data + (image->height - 1 - y) * image->width * image->bitsPerChannel / 8 * 3;

            for (PM_Int64 x = 0 ; x < image->width ; x ++)
            {
                PM_Byte bit = 0;

                switch(context->infoHeader.bitsPerPixel)
                {
                    case 1: bit = (scanLine[x / 8] >> (7 - (x % 8))) & 0x01; break;
                    case 2: bit = (scanLine[x / 4] >> ((3 - (x % 4)) * 2)) & 0x03; break;
                    case 4: bit = (scanLine[x / 2] >> ((1 - (x % 2)) * 4)) & 0x0F; break;
                    case 8: bit = scanLine[x]; break;
                }

                dst[0] = context->colorTable[bit].red;
                dst[1] = context->colorTable[bit].green;
                dst[2] = context->colorTable[bit].blue;

                dst += image->bitsPerChannel / 8 * 3;
            }
        }
    }
    else if (context->infoHeader.bitsPerPixel == 24)
    {
        if (context->infoHeader.compression != 0)
        {
            PM_LogError("PM_ImageBMPDecode: Unsupported compression type(%d) for bitsPerPixel(%d).", context->infoHeader.compression, context->infoHeader.bitsPerPixel);
            return PM_FALSE;
        }

        PM_Size scanLineSize = (context->infoHeader.width * 3 + 3) & ~3; // align to 4 bytes
        for (PM_Int64 y = image->height - 1 ; y >= 0 ; y --)
        {
            PM_Byte* scanLine = context->imageData + y * scanLineSize;
            PM_Byte* dst = image->data + (image->height - 1 - y) * image->width * image->bitsPerChannel / 8 * 3;

            for (PM_Int64 x = 0 ; x < image->width ; x ++)
            {
                dst[0] = scanLine[x * 3 + 2];
                dst[1] = scanLine[x * 3 + 1];
                dst[2] = scanLine[x * 3 + 0];

                dst += image->bitsPerChannel / 8 * 3;
            }
        }
    }
    // NOTE: 16 bits per pixel is not supported yet
    else
    {
        PM_LogError("PM_ImageBMPDecode: Unsupported bits per pixel value(%d).", context->infoHeader.bitsPerPixel);
        return PM_FALSE;
    }

    return PM_TRUE;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImageBMPRead(PM_Stream* stream, PM_Image* image)
{
    PM_BMPContext bmpContext = {0};
    PM_ImageBMPContextInit(&bmpContext);

    if ( ! PM_ImageBMPReadHeader(stream, &bmpContext.header) )
    {
        PM_LogError("PM_ImageBMPRead: Failed to read header.");
        PM_ImageBMPContextDestroy(&bmpContext);
        return PM_FALSE;
    }

    if ( ! PM_ImageBMPReadInfoHeader(stream, &bmpContext.infoHeader) )
    {
        PM_LogError("PM_ImageBMPRead: Failed to read info header.");
        PM_ImageBMPContextDestroy(&bmpContext);
        return PM_FALSE;
    }

    if ( ! PM_ImageBMPReadColorTable(stream, &bmpContext.infoHeader, &bmpContext.colorTable, &bmpContext.colorTableCapacity) )
    {
        PM_LogError("PM_ImageBMPRead: Failed to read color table.");
        PM_ImageBMPContextDestroy(&bmpContext);
        return PM_FALSE;
    }

    if ( ! PM_ImageBMPReadImageData(stream, &bmpContext.header, &bmpContext.imageData, &bmpContext.imageDataCapacity) )
    {
        PM_LogError("PM_ImageBMPRead: Failed to read image data.");
        PM_ImageBMPContextDestroy(&bmpContext);
        return PM_FALSE;
    }

    image->bitsPerChannel = 8;
    image->numChannels = 3;
    image->width = bmpContext.infoHeader.width;
    image->height = bmpContext.infoHeader.height;
    image->dataType = PICOIMEDIA_IMAGE_DATA_TYPE_UINT8;
    image->channelFormat = PICOMEDIA_IMAGE_CHANNEL_FORMAT_RGB;


    if (! PM_ImageAllocate(image, image->width, image->height, image->channelFormat, image->dataType, image->numChannels))
    {
        PM_LogError("PM_ImageBMPRead: Failed to allocate image.");
        PM_ImageBMPContextDestroy(&bmpContext);
        return PM_FALSE;
    }

    if ( ! PM_ImageBMPDecode(&bmpContext, image) )
    {
        PM_LogError("PM_ImageBMPRead: Failed to decode image.");
        PM_ImageBMPContextDestroy(&bmpContext);
        PM_ImageDestroy(image);
        return PM_FALSE;
    }


    PM_ImageBMPContextDestroy(&bmpContext);

    return PM_TRUE;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImageBMPReadFromFile(const PM_Byte* filePath, PM_Image* image)
{
    PM_Assert(filePath != NULL);
    PM_Assert(image != NULL);
    
    PM_Stream stream = {0};
    if ( ! PM_StreamInitFromFile(&stream, filePath, PICOMEDIA_STREAM_FLAG_READ) ) 
    {
        PM_LogError("Failed to initialize stream from file! \n");
        return PM_FALSE;
    }

    PM_Bool readResult = PM_ImageBMPRead(&stream, image);

    PM_StreamDestroy(&stream);

    return readResult;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImageBMPReadFromMemory(PM_Byte* data, PM_Size dataSize, PM_Image* image)
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

    PM_Bool readResult = PM_ImageBMPRead(&stream, image);

    PM_StreamDestroy(&stream);

    return readResult;
}

// -----------------------------------------------------------------------------------------------
