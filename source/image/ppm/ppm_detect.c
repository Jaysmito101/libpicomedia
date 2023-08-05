#include "libpicomedia/image/ppm/ppm.h"

// -----------------------------------------------------------------------------------------------

PM_UInt32 PM_ImagePPMDetect(PM_Stream* stream)
{
    PM_Int8 magicNumber[2] = {0};
    PM_StreamRead(stream, magicNumber, 2);
    if ( (magicNumber[0] == 'P') && (magicNumber[1] == '3'))
    {
        return PICOMEDIA_PPM_FORMAT_P3;
    }
    else if ( (magicNumber[0] == 'P') && (magicNumber[1] == '6'))
    {
        return PICOMEDIA_PPM_FORMAT_P6;
    }
    else
    {
        return PICOMEDIA_PPM_FORMAT_UNKNOWN;
    }
}

// -----------------------------------------------------------------------------------------------

PM_UInt32 PM_ImagePPMDetectFromMemory(PM_Byte* data, PM_Size dataSize)
{
    PM_Stream stream = {0};
    if ( ! PM_StreamInitFromMemory(&stream, data, dataSize, PICOMEDIA_STREAM_FLAG_READ, PM_FALSE) )
    {
        PM_LogError("PM_ImagePPMDetectFromMemory: PM_StreamInitFromMemory failed.");
        return PICOMEDIA_PPM_FORMAT_UNKNOWN;
    }
    PM_UInt32 ppmFormat = PM_ImagePPMDetect(&stream);
    PM_StreamDestroy(&stream);
    return ppmFormat;
}

// -----------------------------------------------------------------------------------------------

PM_UInt32 PM_ImagePPMDetectFromFile(const char* filePath)
{
    PM_Stream stream = {0};
    if ( ! PM_StreamInitFromFile(&stream, filePath, PICOMEDIA_STREAM_FLAG_READ) )
    {
        PM_LogError("PM_ImagePPMDetectFromFile: PM_StreamInitFromFile failed.");
        return PICOMEDIA_PPM_FORMAT_UNKNOWN;
    }
    PM_UInt32 ppmFormat = PM_ImagePPMDetect(&stream);
    PM_StreamDestroy(&stream);
    return ppmFormat;
}

// -----------------------------------------------------------------------------------------------