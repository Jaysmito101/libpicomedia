#include "libpicomedia/libpicomedia.h"

int main(int argc, char** argv, char** envp)
{
    PM_LogInfo("Starting test for Common/Stream");

    PM_Stream testStream = {0};

    PM_LogInfo("Testing Common/Stream/PM_StreamInit ...");
    PM_StreamInit(&testStream);

    const PM_Byte* testFileName = "stream_test.txt";
    PM_LogInfo("Testing Common/Stream/PM_StreamInitFromFile (%s)", testFileName);
    if(!PM_StreamInitFromFile(&testStream, testFileName, PICOMEDIA_STREAM_FLAG_READ))
    {
        PM_LogInfo("Failed to open file");
        return 1;
    }
    
    PM_LogInfo("Testing Common/Stream/PM_StreamGetSourceSize");
    PM_LogInfo("StreamSize: %zu", PM_StreamGetSourceSize(&testStream));

    PM_LogInfo("Testing Common/Stream/PM_StreamRead,PM_StreamPeek");
    PM_LogInfo("PeekedByte : %c", PM_StreamPeekByte(&testStream));
    PM_LogInfo("ReadByte : %c", PM_StreamReadByte(&testStream));

    PM_LogInfo("Testing Common/Stream/PM_StreamRead,PM_StreamPeek,PM_StreamWrite");
    PM_StreamSetCursorPosition(&testStream, 0);
    PM_LogInfo("PeekedByte : %c", PM_StreamPeekByte(&testStream));
    PM_LogInfo("ReadByte : %c", PM_StreamReadByte(&testStream));
    //PM_LogInfo("Writing Byte : %zu", PM_StreamWriteByte(&testStream, 49));


    PM_Byte buffer[1024] = {"Hello Wrold"};

    PM_LogInfo("Testing Common/Stream/PM_StreamInitFromMemory");
    if(!PM_StreamInitFromMemory(&testStream, buffer, sizeof(buffer), PICOMEDIA_STREAM_FLAG_READ, false))
    {
        PM_LogInfo("Failed to open memory stream");
        return 1;
    }
    
    PM_LogInfo("Testing Common/Stream/PM_StreamGetSourceSize");
    PM_LogInfo("StreamSize: %zu", PM_StreamGetSourceSize(&testStream));

    PM_LogInfo("Testing Common/Stream/PM_StreamRead,PM_StreamPeek");
    PM_LogInfo("PeekedByte : %c", PM_StreamPeekByte(&testStream));
    PM_LogInfo("ReadByte : %c", PM_StreamReadByte(&testStream));

    PM_LogInfo("Testing Common/Stream/PM_StreamRead,PM_StreamPeek,PM_StreamWrite");
    PM_StreamSetCursorPosition(&testStream, 0);
    PM_LogInfo("PeekedByte : %c", PM_StreamPeekByte(&testStream));
    PM_LogInfo("ReadByte : %c", PM_StreamReadByte(&testStream));
    //PM_LogInfo("Writing Byte : %zu", PM_StreamWriteByte(&testStream, 49));

    PM_LogInfo("Testing Common/Stream/PM_StreamDestroy");
    PM_StreamDestroy(&testStream);

    PM_LogInfo("Finished test for Common/Stream");
    return 0;

}