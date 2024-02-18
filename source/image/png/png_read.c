#include "libpicomedia/image/png/png.h"

#define PM_TEXT_CHUNKS_CAPACITY 256

// -----------------------------------------------------------------------------------------------

static PM_Bool PM__ImagePNGNextChunk(PM_Stream* stream, PM_Size* chunkSizeOut, PM_UInt8** chunkDataOut)
{
    PM_Assert(stream != NULL);
    PM_Assert(chunkSizeOut != NULL);
    PM_Assert(chunkDataOut != NULL);

    PM_UInt32 chunkLength = 0;
    PM_UInt32 chunkCRC = 0;
    PM_UInt8* chunkData = NULL;


    // Read chunk length
    PM_StreamSetRequireReverse(stream, !PM_IsBigEndian());
    if ( PM_StreamRead(stream, (PM_Byte*)&chunkLength, sizeof(PM_UInt32)) != sizeof(PM_UInt32) )
    {
        // PM_LogWarning("PM__ImagePNGNextChunk: Failed to read chunk length."); // This means end of file
        return PM_FALSE;
    }

    // Allocate memory for chunk data
    chunkData = (PM_UInt8*)PM_Malloc(chunkLength + sizeof(PM_UInt32));
    if ( chunkData == NULL )
    {
        PM_LogWarning("PM__ImagePNGNextChunk: Failed to allocate memory for chunk data.");
        return PM_FALSE;
    }
    

    // Read chunk type
    PM_StreamSetRequireReverse(stream, PM_FALSE);
    if ( PM_StreamRead(stream, (PM_Byte*)chunkData, sizeof(PM_UInt32)) != sizeof(PM_UInt32) )
    {
        PM_LogWarning("PM__ImagePNGNextChunk: Failed to read chunk type.");
        return PM_FALSE;
    }

    // Read chunk data
    if ( chunkLength > 0 )
    {
        // Read chunk data
        if ( PM_StreamRead(stream, (PM_Byte*)(chunkData + sizeof(PM_UInt32)), chunkLength) != chunkLength )
        {
            PM_LogWarning("PM__ImagePNGNextChunk: Failed to read chunk data.");
            PM_Free(chunkData);
            return PM_FALSE;
        }
    }

    // Read chunk CRC
    PM_StreamSetRequireReverse(stream, !PM_IsBigEndian());
    if ( PM_StreamRead(stream, (PM_Byte*)&chunkCRC, sizeof(PM_UInt32)) != sizeof(PM_UInt32) )
    {
        PM_LogWarning("PM__ImagePNGNextChunk: Failed to read chunk CRC.");
        PM_Free(chunkData);
        return PM_FALSE;
    }

    // Verify chunk CRC

    PM_UInt32 crc = PM_CRC32(chunkData, chunkLength + sizeof(PM_UInt32), 0);

    if ( crc != chunkCRC )
    {
        PM_LogWarning("PM__ImagePNGNextChunk: CRC verification failed for chunk[%c%c%c%c]!", 
            (PM_Char)(chunkData[0]),
            (PM_Char)(chunkData[1]),
            (PM_Char)(chunkData[2]),
            (PM_Char)(chunkData[3]));

        PM_Free(chunkData);
        return PM_FALSE;
    }
    
    *chunkSizeOut = chunkLength;
    *chunkDataOut = chunkData;

    return PM_TRUE;
}

// -----------------------------------------------------------------------------------------------

static PM_Bool PM__ImagePNGReadIHDR(PM_PNGContext* context, PM_UInt8* chunkData, PM_Size chunkSize)
{
    if (context->header != NULL)
    {
        PM_LogWarning("PM__ImagePNGReadIHDR: IHDR chunk already read.");
        return PM_FALSE;
    }

    context->header = (PM_PNGHeader*)PM_Malloc(sizeof(PM_PNGHeader));

    PM_Stream stream = {0};
    PM_StreamInitFromMemory(&stream, (PM_Byte*)chunkData, chunkSize, PICOMEDIA_STREAM_FLAG_READ, PM_FALSE);

    PM_StreamSetRequireReverse(&stream, !PM_IsBigEndian());

    if ( PM_StreamRead(&stream, (PM_Byte*)&context->header->width, sizeof(PM_UInt32)) != sizeof(PM_UInt32) )
    {
        PM_LogWarning("PM__ImagePNGReadIHDR: Failed to read width.");
        PM_StreamDestroy(&stream);
        return PM_FALSE;
    }

    if ( PM_StreamRead(&stream, (PM_Byte*)&context->header->height, sizeof(PM_UInt32)) != sizeof(PM_UInt32) )
    {
        PM_LogWarning("PM__ImagePNGReadIHDR: Failed to read height.");
        PM_StreamDestroy(&stream);
        return PM_FALSE;
    }

    if ( PM_StreamRead(&stream, (PM_Byte*)&context->header->bitDepth, sizeof(PM_UInt8)) != sizeof(PM_UInt8) )
    {
        PM_LogWarning("PM__ImagePNGReadIHDR: Failed to read bit depth.");
        PM_StreamDestroy(&stream);
        return PM_FALSE;
    }

    if ( PM_StreamRead(&stream, (PM_Byte*)&context->header->colorType, sizeof(PM_UInt8)) != sizeof(PM_UInt8) )
    {
        PM_LogWarning("PM__ImagePNGReadIHDR: Failed to read color type.");
        PM_StreamDestroy(&stream);
        return PM_FALSE;
    }

    if ( PM_StreamRead(&stream, (PM_Byte*)&context->header->compressionMethod, sizeof(PM_UInt8)) != sizeof(PM_UInt8) )
    {
        PM_LogWarning("PM__ImagePNGReadIHDR: Failed to read compression method.");
        PM_StreamDestroy(&stream);
        return PM_FALSE;
    }

    if ( PM_StreamRead(&stream, (PM_Byte*)&context->header->filterMethod, sizeof(PM_UInt8)) != sizeof(PM_UInt8) )
    {
        PM_LogWarning("PM__ImagePNGReadIHDR: Failed to read filter method.");
        PM_StreamDestroy(&stream);
        return PM_FALSE;
    }

    if ( PM_StreamRead(&stream, (PM_Byte*)&context->header->interlaceMethod, sizeof(PM_UInt8)) != sizeof(PM_UInt8) )
    {
        PM_LogWarning("PM__ImagePNGReadIHDR: Failed to read interlace method.");
        PM_StreamDestroy(&stream);
        return PM_FALSE;
    }

    PM_StreamDestroy(&stream);

    PM_ImagePNGHeaderPrint(context->header);

    return PM_TRUE;
}

// -----------------------------------------------------------------------------------------------

static PM_Bool PM__ImagePNGReadiTXt(PM_PNGContext* context, PM_UInt8* chunkData, PM_Size chunkSize)
{
    (void) chunkSize;

    if (context->header == NULL)
    {
        PM_LogWarning("PM__ImagePNGReadiTXt: IHDR chunk not read.");
        return PM_FALSE;
    }

    if (context->textChunks == NULL)
    {
        context->textChunks = (PM_PNGTextChunk*)PM_Malloc(sizeof(PM_PNGTextChunk) * PM_TEXT_CHUNKS_CAPACITY);
        context->textChunkCount = 0;
        for (PM_Size i = 0; i < PM_TEXT_CHUNKS_CAPACITY; i++)
        {
            PM_ImagePNGTextChunkInit(&context->textChunks[i]);
        }
    }

    if (context->textChunkCount >= PM_TEXT_CHUNKS_CAPACITY)
    {
        PM_LogWarning("PM__ImagePNGReadiTXt: Text chunk capacity exceeded.");
        return PM_FALSE;
    }

    PM_PNGTextChunk* textChunk = &context->textChunks[context->textChunkCount];
    
    memcpy(textChunk->keyword, chunkData, 80);

    PM_ImagePNGTextChunkPrint(textChunk);

    return PM_TRUE;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImagePNGRead(PM_Stream* stream, PM_Image* image)
{
    (void)image;

    PM_PNGContext pngContext = {0};
    PM_ImagePNGContextInit(&pngContext);

    PM_StreamSetRequireReverse(stream, !PM_IsBigEndian());

    PM_StreamSetCursorPosition(stream, 0);

    // Verify PNG magic
    static const PM_UInt8 pngMagic[] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
    PM_UInt8 signature[sizeof(pngMagic)] = { 0 };

    if ( PM_StreamRead(stream, (PM_Byte*)signature, sizeof(pngMagic)) != sizeof(pngMagic) )
	{
		PM_LogWarning("PM_ImagePNGRead: Failed to read PNG signature.");
		PM_ImagePNGContextDestroy(&pngContext);
		return PM_FALSE;
	}

    if ( PM_Memcmp(signature, pngMagic, sizeof(pngMagic)) != 0 )
	{
		PM_LogWarning("PM_ImagePNGRead: Invalid PNG signature.");
		PM_ImagePNGContextDestroy(&pngContext);
		return PM_FALSE;
	}



    PM_Size chunkSize = 0;
    PM_UInt8* chunkData = NULL;
    PM_UInt8* chunkPayloadData = NULL;
    PM_Bool endChunkEncountered = PM_FALSE;


    while (PM__ImagePNGNextChunk(stream, &chunkSize, &chunkData) )
    {
        chunkPayloadData = chunkData + sizeof(PM_UInt32);

        if ( PM_Memcmp(chunkData, "IHDR", 4) == 0)
        {
            if(!PM__ImagePNGReadIHDR(&pngContext, chunkPayloadData, chunkSize) )
            {
                PM_LogWarning("PM_ImagePNGRead: Failed to read IHDR chunk.");
                PM_ImagePNGContextDestroy(&pngContext);
                PM_Free(chunkData);
                return PM_FALSE;
            }
        }
        else if ( PM_Memcmp(chunkData, "PLTE", 4) == 0 )
        {
            PM_LogInfo("PLTE Chunk");
        }
        else if ( PM_Memcmp(chunkData, "IDAT", 4) == 0 )
        {
            PM_LogInfo("IDAT Chunk");
        }
        else if ( PM_Memcmp(chunkData, "IEND", 4) == 0 )
        {
            PM_LogInfo("IEND Chunk");
            endChunkEncountered = PM_TRUE;
            break;
        }
        else if (PM_Memcmp(chunkData, "iTXt", 4) == 0)
        {
            if(!PM__ImagePNGReadiTXt(&pngContext, chunkPayloadData, chunkSize))
            {
                PM_LogWarning("PM_ImagePNGRead: Failed to read iTXt chunk.");
                PM_ImagePNGContextDestroy(&pngContext);
                PM_Free(chunkData);
                return PM_FALSE;
            }
        }
        else
        {
            PM_LogInfo("PM_ImagePNGRead: Skipping Unknown Chunk {%c%c%c%c}", 
                (PM_Char)(chunkData[0]),
                (PM_Char)(chunkData[1]),
                (PM_Char)(chunkData[2]),
                (PM_Char)(chunkData[3]));
        }

        if (chunkData != NULL) {
            PM_Free(chunkData);
            chunkData = NULL;
        }

    }
    
    if ( !endChunkEncountered )
    {
        PM_LogWarning("PM_ImagePNGRead: Error in Parsing PNG image!");
        PM_ImagePNGContextDestroy(&pngContext);
        return PM_FALSE;
    }



    // if (!PM_ImageAllocate(image, image->width, image->height, image->channelFormat, image->dataType, image->numChannels))
    // {
    //     PM_LogWarning("PM_ImagePNGRead: Failed to allocate image.");
    //     PM_ImagePNGContextDestroy(&pngContext);
    //     return PM_FALSE;
    // }

    PM_ImagePNGContextDestroy(&pngContext);

    return PM_TRUE;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImagePNGReadFromFile(const PM_Byte* filePath, PM_Image* image)
{
    PM_Assert(filePath != NULL);
    PM_Assert(image != NULL);
    
    PM_Stream stream = {0};
    if ( ! PM_StreamInitFromFile(&stream, filePath, PICOMEDIA_STREAM_FLAG_READ) ) 
    {
        PM_LogWarning("Failed to initialize stream from file! \n");
        return PM_FALSE;
    }

    PM_Bool readResult = PM_ImagePNGRead(&stream, image);

    PM_StreamDestroy(&stream);

    return readResult;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImagePNGReadFromMemory(PM_Byte* data, PM_Size dataSize, PM_Image* image)
{
    PM_Assert(data != NULL);
    PM_Assert(image != NULL);
    PM_Assert(dataSize > 0);

    PM_Stream stream = {0};
    if ( ! PM_StreamInitFromMemory(&stream, data, dataSize, PICOMEDIA_STREAM_FLAG_READ, false) ) 
    {
        PM_LogWarning("Failed to initialize stream from memory! \n");
        return PM_FALSE;
    }

    PM_Bool readResult = PM_ImagePNGRead(&stream, image);

    PM_StreamDestroy(&stream);

    return readResult;
}

// -----------------------------------------------------------------------------------------------
