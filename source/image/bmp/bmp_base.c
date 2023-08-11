#include "libpicomedia/image/bmp/bmp.h"

// -----------------------------------------------------------------------------------------------

void PM_ImageBMPHeaderInit(PM_BMPHeader* header)
{
    PM_Assert(header != NULL);

    header->signature = 0;
    header->fileSize = 0;
    header->reserved = 0;
    header->dataOffset = 0;
}

// -----------------------------------------------------------------------------------------------

void PM_ImageBMPHeaderPrint(const PM_BMPHeader* header)
{
    PM_Assert(header != NULL);

    PM_LogInfo(
        "\n"
        "PM_BMPHeader {\n"
        "    signature: %d\n"
        "    fileSize: %d\n"
        "    reserved: %d\n"
        "    dataOffset: %d\n"
        "}",
        header->signature,
        header->fileSize,
        header->reserved,
        header->dataOffset
    );
}

// -----------------------------------------------------------------------------------------------

void PM_ImageBMPInfoHeaderInit(PM_BMPInfoHeader* infoHeader)
{
    PM_Assert(infoHeader != NULL);

    infoHeader->headerSize = 0;
    infoHeader->width = 0;
    infoHeader->height = 0;
    infoHeader->planes = 0;
    infoHeader->bitsPerPixel = 0;
    infoHeader->compression = 0;
    infoHeader->imageSize = 0;
    infoHeader->xPixelsPerMeter = 0;
    infoHeader->yPixelsPerMeter = 0;
    infoHeader->colorsUsed = 0;
    infoHeader->colorsImportant = 0;
}

// -----------------------------------------------------------------------------------------------

void PM_ImageBMPInfoHeaderPrint(const PM_BMPInfoHeader* infoHeader)
{
    PM_Assert(infoHeader != NULL);

    PM_LogInfo(
        "\n"
        "PM_BMPInfoHeader {\n"
        "    headerSize: %d\n"
        "    width: %d\n"
        "    height: %d\n"
        "    planes: %d\n"
        "    bitsPerPixel: %d\n"
        "    compression: %d\n"
        "    imageSize: %d\n"
        "    xPixelsPerMeter: %d\n"
        "    yPixelsPerMeter: %d\n"
        "    colorsUsed: %d\n"
        "    colorsImportant: %d\n"
        "}",
        infoHeader->headerSize,
        infoHeader->width,
        infoHeader->height,
        infoHeader->planes,
        infoHeader->bitsPerPixel,
        infoHeader->compression,
        infoHeader->imageSize,
        infoHeader->xPixelsPerMeter,
        infoHeader->yPixelsPerMeter,
        infoHeader->colorsUsed,
        infoHeader->colorsImportant
    );
}

// -----------------------------------------------------------------------------------------------

void PM_ImageBMPColorTableItemInit(PM_BMPColorTableItem* item)
{
    PM_Assert(item != NULL);

    item->blue = 0;
    item->green = 0;
    item->red = 0;
    item->reserved = 0;
}

// -----------------------------------------------------------------------------------------------

void PM_ImageBMPColorTableItemPrint(const PM_BMPColorTableItem* item)
{
    PM_Assert(item != NULL);

    PM_LogInfo(
        "\n"
        "PM_BMPColorTableItem {\n"
        "    blue: %d\n"
        "    green: %d\n"
        "    red: %d\n"
        "    reserved: %d\n"
        "}",
        item->blue,
        item->green,
        item->red,
        item->reserved
    );
}

// -----------------------------------------------------------------------------------------------

void PM_ImageBMPContextInit(PM_BMPContext* context)
{
    PM_Assert(context != NULL);

    PM_ImageBMPHeaderInit(&context->header);
    PM_ImageBMPInfoHeaderInit(&context->infoHeader);
    
    context->colorTable = NULL;
    context->colorTableCapacity = 0;
    context->imageData = NULL;
    context->imageDataCapacity = 0;
}

// -----------------------------------------------------------------------------------------------

void PM_ImageBMPContextDestroy(PM_BMPContext* context)
{
    PM_Assert(context != NULL);

    if (context->colorTable != NULL)
    {
        PM_Free(context->colorTable);
    }

    if (context->imageData != NULL)
    {
        PM_Free(context->imageData);
    }

    PM_ImageBMPContextInit(context);
}

// -----------------------------------------------------------------------------------------------
