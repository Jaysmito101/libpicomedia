#include "libpicomedia/image/bmp/bmp.h"

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImageBMPWriteHeader(PM_Stream* stream, const PM_BMPHeader* header)
{
    PM_Assert(stream != NULL);
    PM_Assert(header != NULL);

    PM_Bool writeResult = PM_TRUE;

    PM_StreamSetRequireReverse(stream, PM_FALSE);

    if ( ! PM_StreamWriteUInt16(stream, header->signature) )
    {
        PM_LogError("Failed to write signature! \n");
        writeResult = PM_FALSE;
    }

    PM_StreamSetRequireReverse(stream, PM_IsBigEndian());

    if ( ! PM_StreamWriteUInt32(stream, header->fileSize) )
    {
        PM_LogError("Failed to write file size! \n");
        writeResult = PM_FALSE;
    }

    if ( ! PM_StreamWriteUInt32(stream, header->reserved) )
    {
        PM_LogError("Failed to write reserved1! \n");
        writeResult = PM_FALSE;
    }

    if ( ! PM_StreamWriteUInt32(stream, header->dataOffset) )
    {
        PM_LogError("Failed to write image data offset! \n");
        writeResult = PM_FALSE;
    }

    return writeResult;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImageBMPWriteInfoHeader(PM_Stream* stream, const PM_BMPInfoHeader* infoHeader)
{
    PM_Assert(stream != NULL);
    PM_Assert(infoHeader != NULL);

    PM_StreamSetRequireReverse(stream, PM_IsBigEndian());

    PM_Bool writeResult = PM_TRUE;

    if ( ! PM_StreamWriteUInt32(stream, infoHeader->headerSize) )
    {
        PM_LogError("Failed to write info header size! \n");
        writeResult =  PM_FALSE;
    }

    if ( ! PM_StreamWriteInt32(stream, infoHeader->width) )
    {
        PM_LogError("Failed to write image width! \n");
        writeResult =  PM_FALSE;
    }

    if ( ! PM_StreamWriteInt32(stream, infoHeader->height) )
    {
        PM_LogError("Failed to write image height! \n");
        writeResult =  PM_FALSE;
    }

    if ( ! PM_StreamWriteUInt16(stream, infoHeader->planes) )
    {
        PM_LogError("Failed to write planes! \n");
        writeResult =  PM_FALSE;
    }

    if ( ! PM_StreamWriteUInt16(stream, infoHeader->bitsPerPixel) )
    {
        PM_LogError("Failed to write bits per pixel! \n");
        writeResult =  PM_FALSE;
    }

    if ( ! PM_StreamWriteUInt32(stream, infoHeader->compression) )
    {
        PM_LogError("Failed to write compression! \n");
        writeResult =  PM_FALSE;
    }

    if ( ! PM_StreamWriteUInt32(stream, infoHeader->imageSize) )
    {
        PM_LogError("Failed to write image size! \n");
        writeResult =  PM_FALSE;
    }

    if ( ! PM_StreamWriteInt32(stream, infoHeader->xPixelsPerMeter) )
    {
        PM_LogError("Failed to write x pixels per meter! \n");
        writeResult =  PM_FALSE;
    }

    if ( ! PM_StreamWriteInt32(stream, infoHeader->yPixelsPerMeter) )
    {
        PM_LogError("Failed to write y pixels per meter! \n");
        writeResult =  PM_FALSE;
    }

    if ( ! PM_StreamWriteUInt32(stream, infoHeader->colorsUsed) )
    {
        PM_LogError("Failed to write colors used! \n");
        writeResult = PM_FALSE;
    }

    if ( ! PM_StreamWriteUInt32(stream, infoHeader->colorsImportant) )
    {
        PM_LogError("Failed to write colors important! \n");
        writeResult = PM_FALSE;
    }

    return writeResult;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImageBMPWriteColorTable(PM_Stream* stream, const PM_BMPColorTableItem* colorTable, PM_Size colorTableSize)
{
    PM_Assert(stream != NULL);

    if ( colorTable == NULL || colorTableSize == 0 )
    {
        return PM_TRUE;
    }

    PM_Bool writeResult = PM_TRUE;

    PM_Size colorTableSizeInBytes = colorTableSize * sizeof(PM_BMPColorTableItem);

    if ( PM_StreamWrite(stream, (const PM_Byte*)colorTable, colorTableSizeInBytes) != colorTableSizeInBytes )
    {
        PM_LogError("Failed to write color table! \n");
        writeResult = PM_FALSE;
    }

    return writeResult;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImageBMPEncode(const PM_Image* image, PM_BMPContext* context)
{
    PM_Assert(image != NULL);
    PM_Assert(image->channelFormat == PICOMEDIA_IMAGE_CHANNEL_FORMAT_BGR); // BMP exporter only allows BGR format
    PM_Assert(image->dataType == PICOIMEDIA_IMAGE_DATA_TYPE_UINT8); // BMP exporter only allows 8-bit per channel
    PM_Assert(image->bitsPerChannel == 8); // BMP exporter only allows 8-bit per channel
    PM_Assert(image->numChannels == 3); // BMP exporter only allows 3 channels (BGR)
    PM_Assert(context != NULL);

    PM_ImageBMPContextInit(context); 

    PM_Size scanLineSize = image->width * image->bitsPerChannel / 8 * image->numChannels;
    PM_Size scanLinePadding = (4 - (scanLineSize % 4)) % 4;
    PM_Size scanLineSizeWithPadding = scanLineSize + scanLinePadding;
    PM_Size imageDataSize = scanLineSizeWithPadding * image->height;

    // prepare header
    ((PM_Byte*)&context->header.signature)[0] = 'B';
    ((PM_Byte*)&context->header.signature)[1] = 'M';
    context->header.fileSize = 14 /*sizeof(PM_BMPHeader)*/ + 40 /*sizeof(PM_BMPInfoHeader)*/ + (PM_UInt32)imageDataSize;
    context->header.reserved = 0;
    context->header.dataOffset = 14 /*sizeof(PM_BMPHeader)*/ + 40 /*sizeof(PM_BMPInfoHeader)*/; // for our case, we won't have color table as we are using 24-bit color depth

    // prepare info header
    context->infoHeader.headerSize = sizeof(PM_BMPInfoHeader);
    context->infoHeader.width = image->width;
    context->infoHeader.height = image->height;
    context->infoHeader.planes = 1;
    context->infoHeader.bitsPerPixel = 24; // for now we only support 24-bit color depth
    context->infoHeader.compression = 0; // no compression
    context->infoHeader.imageSize = (PM_UInt32)imageDataSize;
    context->infoHeader.xPixelsPerMeter = 2835; // 72 DPI
    context->infoHeader.yPixelsPerMeter = 2835; // 72 DPI
    context->infoHeader.colorsUsed = 0; // no color table
    context->infoHeader.colorsImportant = 0; // all colors are important

    // prepare color table
    context->colorTable = NULL;
    context->colorTableCapacity = 0;

    // prepare data
    context->imageData = (PM_Byte*)PM_Malloc(imageDataSize);
    if ( context->imageData == NULL )
    {
        PM_LogError("Failed to allocate memory for image data! \n");
        return PM_FALSE;
    }
    PM_Memset(context->imageData, 0, imageDataSize);
    context->imageDataCapacity = imageDataSize;    

    // copy image data
    for ( PM_Size y = 0; y < image->height; ++y )
    {
        PM_Size srcOffset = (image->height - 1 - y) * image->width * image->numChannels;
        PM_Size dstOffset = y * scanLineSizeWithPadding;

        PM_Memcpy(context->imageData + dstOffset, image->data + srcOffset, scanLineSize);
    }


    return PM_TRUE;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImageBMPWrite(const PM_Image* image, PM_Stream* stream)
{
    PM_Assert(stream != NULL);
    PM_Assert(image != NULL);

    PM_BMPContext context = {0};
    PM_ImageBMPContextInit(&context);

    if ( ! PM_ImageBMPEncode(image, &context) )
    {
        PM_LogError("Failed to encode image! \n");
        return PM_FALSE;
    }

    PM_Bool writeResult = PM_TRUE;

    PM_StreamSetCursorPosition(stream, 0);
    PM_StreamSetRequireReverse(stream, PM_IsBigEndian());

    if ( ! PM_ImageBMPWriteHeader(stream, &context.header) )
    {
        PM_LogError("Failed to write header! \n");
        writeResult = PM_FALSE;
    }

    if ( ! PM_ImageBMPWriteInfoHeader(stream, &context.infoHeader) )
    {
        PM_LogError("Failed to write info header! \n");
        writeResult = PM_FALSE;
    }

    if ( ! PM_ImageBMPWriteColorTable(stream, context.colorTable, context.colorTableCapacity) )
    {
        PM_LogError("Failed to write color table! \n");
        writeResult = PM_FALSE;
    }

    if ( PM_StreamWrite(stream, context.imageData, context.imageDataCapacity) != context.imageDataCapacity )
    {
        PM_LogError("Failed to write image data! \n");
        writeResult = PM_FALSE;
    }


    PM_ImageBMPContextDestroy(&context);
    return PM_TRUE;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImageBMPWriteToFile(const PM_Image* image, const PM_Byte* filePath)
{
    PM_Assert(filePath != NULL);
    PM_Assert(image != NULL);

    PM_Stream stream = {0};
    if ( ! PM_StreamInitFromFile(&stream, filePath, PICOMEDIA_STREAM_FLAG_WRITE) ) 
    {
        PM_LogError("Failed to initialize stream from file! \n");
        return PM_FALSE;
    }

    PM_Bool writeResult = PM_ImageBMPWrite(image, &stream);

    PM_StreamDestroy(&stream);

    return writeResult;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImageBMPWriteToMemory(const PM_Image* image, PM_Byte* data, PM_Size* dataSize, PM_Size maxDataSize)
{
    PM_Assert(data != NULL);
    PM_Assert(maxDataSize > 0);
    PM_Assert(image != NULL);


    PM_Stream stream = {0};
    if ( ! PM_StreamInitFromMemory(&stream, data, maxDataSize, PICOMEDIA_STREAM_FLAG_WRITE, false) ) 
    {
        PM_LogError("Failed to initialize stream from memory! \n");
        return PM_FALSE;
    }

    PM_Bool writeResult = PM_ImageBMPWrite(image, &stream);

    if (writeResult && dataSize != NULL)
    {
        *dataSize = PM_StreamGetCursorPosition(&stream);
    }

    PM_StreamDestroy(&stream);

    return writeResult;
}

// -----------------------------------------------------------------------------------------------
