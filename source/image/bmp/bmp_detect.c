#include "libpicomedia/image/bmp/bmp.h"

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImageBMPDetect(PM_Stream* stream)
{
    PM_Assert(stream != NULL);

    static PM_Byte magicNumber[2] = {0};
    PM_StreamRead(stream, magicNumber, 2);

    return ( (magicNumber[0] == 'B') && (magicNumber[1] == 'M'));
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImageBMPDetectFromMemory(PM_Byte* data, PM_Size dataSize)
{
    PM_Assert(data != NULL);
    PM_Assert(dataSize > 0);

    PM_Stream stream = {0};
    if ( ! PM_StreamInitFromMemory(&stream, data, dataSize, PICOMEDIA_STREAM_FLAG_READ, PM_FALSE) )
    {
        PM_LogWarning("PM_ImageBMPDetectFromMemory: PM_StreamInitFromMemory failed.");
        return PM_FALSE;
    }
    PM_Bool detectionResult = PM_ImageBMPDetect(&stream);
    PM_StreamDestroy(&stream);
    return detectionResult;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImageBMPDetectFromFile(const char* filePath)
{
    PM_Assert(filePath != NULL);

    PM_Stream stream = {0};
    if ( ! PM_StreamInitFromFile(&stream, filePath, PICOMEDIA_STREAM_FLAG_READ) )
    {
        PM_LogWarning("PM_ImageBMPDetectFromFile: PM_StreamInitFromFile failed.");
        return PM_FALSE;
    }
    PM_Bool detectionResult = PM_ImageBMPDetect(&stream);
    PM_StreamDestroy(&stream);
    return detectionResult;
}

// -----------------------------------------------------------------------------------------------