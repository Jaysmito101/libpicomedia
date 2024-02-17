#include "libpicomedia/image/png/png.h"

// -----------------------------------------------------------------------------------------------

void PM_ImagePNGHeaderInit(PM_PNGHeader* header)
{
    PM_Assert(header != NULL);

    header->width = 0;
    header->height = 0;
    header->bitDepth = 0;
    header->colorType = 0;
    header->compressionMethod = 0;
    header->filterMethod = 0;
    header->interlaceMethod = 0;
}

// -----------------------------------------------------------------------------------------------

void PM_ImagePNGHeaderPrint(const PM_PNGHeader* header)
{
    PM_Assert(header != NULL);

    PM_LogInfo(
        "\n"
        "PM_PNGHeader (%s) {\n"
        "    width: %d\n"
        "    height: %d\n"
        "    bitDepth: %d\n"
        "    colorType: %d\n"
        "    compressionMethod: %d\n"
        "    filterMethod: %d\n"
        "    interlaceMethod: %d\n"
        "}",
        PM_ImagePNGHeaderIsValid(header) ? "valid" : "invalid",
        header->width,
        header->height,
        header->bitDepth,
        header->colorType,
        header->compressionMethod,
        header->filterMethod,
        header->interlaceMethod
    );
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImagePNGHeaderIsValid(const PM_PNGHeader* header)
{
    PM_Assert(header != NULL);

    if (header->width == 0 || header->height == 0)
    {
        return PM_FALSE;
    }

    switch(header->colorType)
    {
        // Grayscale
        case 0: if (header->bitDepth != 1 && header->bitDepth != 2 && header->bitDepth != 4 && header->bitDepth != 8 && header->bitDepth != 16) return PM_FALSE; break;
        // Truecolor
        case 2: if (header->bitDepth != 8 && header->bitDepth != 16) return PM_FALSE; break;
        // Indexed-color
        case 3: if (header->bitDepth != 1 && header->bitDepth != 2 && header->bitDepth != 4 && header->bitDepth != 8) return PM_FALSE; break;
        // Grayscale with alpha
        case 4: if (header->bitDepth != 8 && header->bitDepth != 16) return PM_FALSE; break;
        // Truecolor with alpha
        case 6: if (header->bitDepth != 8 && header->bitDepth != 16) return PM_FALSE; break;
        // Invalid color type
        default: return PM_FALSE;
    }

    if (header->compressionMethod != 0)
    {
        return PM_FALSE;
    }

    if (header->filterMethod != 0)
    {
        return PM_FALSE;
    }

    if (header->interlaceMethod != 0 && header->interlaceMethod != 1)
    {
        return PM_FALSE;
    }

    return PM_TRUE;
}

// -----------------------------------------------------------------------------------------------

void  PM_ImagePNGPalletteInit(PM_PNGPallette* palette)
{
    PM_Assert(palette != NULL);

    for (PM_UInt32 i = 0; i < 256; i++)
    {
        palette->data[i][0] = 0;
        palette->data[i][1] = 0;
        palette->data[i][2] = 0;
    }
    palette->size = 0;    
}

// -----------------------------------------------------------------------------------------------

void  PM_ImagePNGPallettePrint(const PM_PNGPallette* palette)
{
    PM_Assert(palette != NULL);

    PM_LogInfo(
        "\n"
        "PM_PNGPallette {\n"
        "    size: %zu\n"
        "}",
        palette->size
    );
}

// -----------------------------------------------------------------------------------------------

void  PM_ImagePNGTransparencyInit(PM_PNGTransparency* transparency)
{
    PM_Assert(transparency != NULL);

    memset(transparency, 0, sizeof(PM_PNGTransparency));
}

// -----------------------------------------------------------------------------------------------

void  PM_ImagePNGTransparencyPrint(const PM_PNGTransparency* transparency, PM_UInt8 colorType)
{
    PM_Assert(transparency != NULL);

    switch (colorType)
    {
        case 0:
        {
            PM_LogInfo(
                "\n"
                "PM_PNGTransparency {\n"
                "    gray: %d\n"
                "}",
                transparency->gray
            );
            break;
        }
        case 2:
        {
            PM_LogInfo(
                "\n"
                "PM_PNGTransparency {\n"
                "    red: %d\n"
                "    green: %d\n"
                "    blue: %d\n"
                "}",
                transparency->rgb[0],
                transparency->rgb[0],
                transparency->rgb[0]
            );
            break;
        }
        case 3:
        {
            PM_LogInfo(
                "\n"
                "PM_PNGTransparency {\n"
                "    alpha: 0-256\n"
                "}"
            );
            break;
        }
        default:
        {
            PM_LogInfo(
                "\n"
                "PM_PNGTransparency {\n"
                "    Unknown/Not needed\n"
                "}"
            );
            break;
        }
    }
}

// -----------------------------------------------------------------------------------------------

void  PM_ImagePNGChrmInit(PM_PNGChrm* chrm)
{
    PM_Assert(chrm != NULL);

    chrm->whitePointX = 0;
    chrm->whitePointY = 0;
    chrm->redX = 0;
    chrm->redY = 0;
    chrm->greenX = 0;
    chrm->greenY = 0;
    chrm->blueX = 0;
    chrm->blueY = 0;
}

// -----------------------------------------------------------------------------------------------

void  PM_ImagePNGChrmPrint(const PM_PNGChrm* chrm)
{
    PM_Assert(chrm != NULL);

    PM_LogInfo(
        "\n"
        "PM_PNGChrm {\n"
        "    whitePointX: %f\n"
        "    whitePointY: %f\n"
        "    redX: %f\n"
        "    redY: %f\n"
        "    greenX: %f\n"
        "    greenY: %f\n"
        "    blueX: %f\n"
        "    blueY: %f\n"
        "}",
        chrm->whitePointX,
        chrm->whitePointY,
        chrm->redX,
        chrm->redY,
        chrm->greenX,
        chrm->greenY,
        chrm->blueX,
        chrm->blueY
    );
}

// -----------------------------------------------------------------------------------------------

void  PM_ImagePNGICCProfileInit(PM_PNGICCProfile* iccProfile)
{
    PM_Assert(iccProfile != NULL);

    sprintf(iccProfile->name, "Unknown");
    iccProfile->compressedData = NULL;
    iccProfile->compressedDataSize = 0;
    iccProfile->compressionMethod = 0;
}

// -----------------------------------------------------------------------------------------------

void  PM_ImagePNGICCProfileDestroy(PM_PNGICCProfile* iccProfile)
{
    PM_Assert(iccProfile != NULL);

    if (iccProfile->compressedData != NULL)
    {
        PM_Free(iccProfile->compressedData);
        iccProfile->compressedData = NULL;
    }
}

// -----------------------------------------------------------------------------------------------

void  PM_ImagePNGICCProfilePrint(const PM_PNGICCProfile* iccProfile)
{
    PM_Assert(iccProfile != NULL);

    PM_LogInfo(
        "\n"
        "PM_PNGICCProfile (not fully supported) {\n"
        "    name: %s\n"
        "    compressedDataSize: %zu\n"
        "    compressionMethod: %d\n"
        "}",
        iccProfile->name,
        iccProfile->compressedDataSize,
        iccProfile->compressionMethod
    );
}

// -----------------------------------------------------------------------------------------------

void  PM_ImagePNGTextChunkInit(PM_PNGTextChunk* textChunk)
{
    PM_Assert(textChunk != NULL);

    sprintf(textChunk->keyword, "Unknown");
    textChunk->text = NULL;
}

// -----------------------------------------------------------------------------------------------

void  PM_ImagePNGTextChunkDestroy(PM_PNGTextChunk* textChunk)
{
    PM_Assert(textChunk != NULL);

    if (textChunk->text != NULL)
    {
        PM_Free(textChunk->text);
        textChunk->text = NULL;
    }
}

// -----------------------------------------------------------------------------------------------

void  PM_ImagePNGTextChunkPrint(const PM_PNGTextChunk* textChunk)
{
    PM_Assert(textChunk != NULL);

    PM_LogInfo(
        "\n"
        "PM_PNGTextChunk {\n"
        "    keyword: %s\n"
        "    text: %s\n"
        "}",
        textChunk->keyword,
        textChunk->text
    );
}

// -----------------------------------------------------------------------------------------------

void  PM_ImagePNGTimeChunkInit(PM_PNGTimeChunk* timeChunk)
{
    PM_Assert(timeChunk != NULL);

    timeChunk->year = 0;
    timeChunk->month = 0;
    timeChunk->day = 0;
    timeChunk->hour = 0;
    timeChunk->minute = 0;
    timeChunk->second = 0;
}

// -----------------------------------------------------------------------------------------------

void  PM_ImagePNGTimeChunkPrint(const PM_PNGTimeChunk* timeChunk)
{
    PM_Assert(timeChunk != NULL);

    PM_LogInfo(
        "\n"
        "PM_PNGTimeChunk {\n"
        "    year: %d\n"
        "    month: %d\n"
        "    day: %d\n"
        "    hour: %d\n"
        "    minute: %d\n"
        "    second: %d\n"
        "}",
        timeChunk->year,
        timeChunk->month,
        timeChunk->day,
        timeChunk->hour,
        timeChunk->minute,
        timeChunk->second
    );
}

// -----------------------------------------------------------------------------------------------

void  PM_ImagePNGContextInit(PM_PNGContext* context)
{
    PM_Assert(context != NULL);

    context->header = NULL;
    context->palette = NULL;
    context->transparency = NULL;
    context->chrm = NULL;
    context->gamma = 1.0f;
    context->iccProfile = NULL;
    context->sRGBRenderingIntent = 0;
    context->textChunks = NULL;
    context->textChunkCount = 0;
    context->exifData = NULL;
    context->exifDataSize = 0;
    context->timeChunk = NULL;
    context->rawData = NULL;
    context->dataSize = 0;
}

// -----------------------------------------------------------------------------------------------

void  PM_ImagePNGContextDestroy(PM_PNGContext* context)
{
    PM_Assert(context != NULL);

    if (context->header != NULL)
    {
        PM_Free(context->header);
        context->header = NULL;
    }

    if (context->palette != NULL)
    {
        PM_Free(context->palette);
        context->palette = NULL;
    }

    if (context->transparency != NULL)
    {
        PM_Free(context->transparency);
        context->transparency = NULL;
    }

    if (context->chrm != NULL)
    {
        PM_Free(context->chrm);
        context->chrm = NULL;
    }

    if (context->iccProfile != NULL)
    {
        PM_ImagePNGICCProfileDestroy(context->iccProfile);
        PM_Free(context->iccProfile);
        context->iccProfile = NULL;
    }

    if (context->textChunks != NULL)
    {
        for (PM_UInt32 i = 0; i < context->textChunkCount; i++)
        {
            PM_ImagePNGTextChunkDestroy(&context->textChunks[i]);
        }
        PM_Free(context->textChunks);
        context->textChunks = NULL;
    }

    if (context->exifData != NULL)
    {
        PM_Free(context->exifData);
        context->exifData = NULL;
    }

    if (context->timeChunk != NULL)
    {
        PM_Free(context->timeChunk);
        context->timeChunk = NULL;
    }

    if (context->rawData != NULL)
    {
        PM_Free(context->rawData);
        context->rawData = NULL;
    }

    context->dataSize = 0;

}

// -----------------------------------------------------------------------------------------------

void  PM_ImagePNGContextPrint(const PM_PNGContext* context)
{
    PM_Assert(context != NULL);

    PM_LogInfo(
        "\n"
        "PM_PNGContext {\n"
        "    header: %s\n"
        "    palette: %s\n"
        "    transparency: %s\n"
        "    chrm: %s\n"
        "    gamma: %f\n"
        "    iccProfile: %s\n"
        "    sRGBRenderingIntent: %d\n"
        "    textChunks: %s\n"
        "    exifDataSize: %zu\n"
        "    timeChunk: %s\n"
        "    dataSize: %zu\n"
        "}",
        context->header != NULL ? "exists" : "NULL",
        context->palette != NULL ? "exists" : "NULL",
        context->transparency != NULL ? "exists" : "NULL",
        context->chrm != NULL ? "exists" : "NULL",
        context->gamma,
        context->iccProfile != NULL ? "exists" : "NULL",
        context->sRGBRenderingIntent,
        context->textChunks != NULL ? "exists" : "NULL",
        context->exifDataSize,
        context->timeChunk != NULL ? "exists" : "NULL",
        context->dataSize
    );

    if (context->header != NULL)
    {
        PM_ImagePNGHeaderPrint(context->header);
    }

    if (context->palette != NULL)
    {
        PM_ImagePNGPallettePrint(context->palette);
    }

    if (context->transparency != NULL)
    {
        PM_ImagePNGTransparencyPrint(context->transparency, context->header->colorType);
    }

    if (context->chrm != NULL)
    {
        PM_ImagePNGChrmPrint(context->chrm);
    }

    if (context->iccProfile != NULL)
    {
        PM_ImagePNGICCProfilePrint(context->iccProfile);
    }

    if (context->textChunks != NULL)
    {
        for (PM_UInt32 i = 0; i < context->textChunkCount; i++)
        {
            PM_ImagePNGTextChunkPrint(&context->textChunks[i]);
        }
    }

    if (context->timeChunk != NULL)
    {
        PM_ImagePNGTimeChunkPrint(context->timeChunk);
    }


}

// -----------------------------------------------------------------------------------------------

