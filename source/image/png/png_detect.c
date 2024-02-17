#include "libpicomedia/image/png/png.h"

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImagePNGDetect(PM_Stream* stream)
{
    PM_Assert(stream != NULL);

    static const PM_Byte pngMagic[] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
    static PM_Byte magicNumber[sizeof(pngMagic)] = {0};

    PM_StreamRead(stream, magicNumber, sizeof(magicNumber));
    
    return ( PM_Memcmp(magicNumber, pngMagic, sizeof(magicNumber)) == 0 );
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImagePNGDetectFromMemory(PM_Byte* data, PM_Size dataSize)
{
    PM_Assert(data != NULL);
    PM_Assert(dataSize > 0);

    PM_Stream stream = {0};
    if ( ! PM_StreamInitFromMemory(&stream, data, dataSize, PICOMEDIA_STREAM_FLAG_READ, PM_FALSE) )
    {
        PM_LogError("PM_ImagePNGDetectFromMemory: PM_StreamInitFromMemory failed.");
        return PM_FALSE;
    }
    PM_Bool detectionResult = PM_ImagePNGDetect(&stream);
    PM_StreamDestroy(&stream);
    return detectionResult;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImagePNGDetectFromFile(const PM_Char* filePath)
{
    PM_Assert(filePath != NULL);

    PM_Stream stream = {0};
    if ( ! PM_StreamInitFromFile(&stream, filePath, PICOMEDIA_STREAM_FLAG_READ) )
    {
        PM_LogError("PM_ImagePNGDetectFromFile: PM_StreamInitFromFile failed.");
        return PM_FALSE;
    }
    PM_Bool detectionResult = PM_ImagePNGDetect(&stream);
    PM_StreamDestroy(&stream);
    return detectionResult;
}

// -----------------------------------------------------------------------------------------------