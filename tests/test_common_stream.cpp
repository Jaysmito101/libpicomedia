#include <iostream>

#include "libpicomedia/libpicomedia.hpp"

namespace pm = picomedia;

int main(int argc, char** argv, char** envp)
{
    
    PM_LogInfo("Starting test for Common/Stream");


    PM_LogInfo("Testing Common/Stream/PM_StreamInit ...");
    pm::Stream testStream;

    const PM_Byte* testFileName = "stream_test.txt";
    PM_LogInfo("Testing Common/Stream/PM_StreamInitFromFile (%s)", testFileName);
    testStream = pm::Stream(testFileName, pm::Stream::FlagRead);
    
    PM_LogInfo("Testing Common/Stream/PM_StreamGetSourceSize");
    PM_LogInfo("StreamSize: %zu", testStream.GetSize());

    PM_LogInfo("Testing Common/Stream/PM_StreamRead,PM_StreamPeek");
    PM_LogInfo("PeekedByte : %c", testStream.Peek<PM_Byte>());
    PM_LogInfo("ReadByte : %c", testStream.Peek<PM_Byte>());

    PM_LogInfo("Testing Common/Stream/PM_StreamRead,PM_StreamPeek,PM_StreamWrite");
    testStream.SetCursorPosition(0);
    PM_LogInfo("PeekedByte : %c", testStream.Peek<PM_Byte>());
    PM_LogInfo("ReadByte : %c", testStream.Peek<PM_Byte>());
    //PM_LogInfo("Writing Byte : %zu", PM_StreamWriteByte(&testStream, 49));

    PM_LogInfo("Testing Common/Stream/PM_StreamDestroy");

    PM_Byte buffer[1024] = {"Hello Wrold"};

    PM_LogInfo("Testing Common/Stream/PM_StreamInitFromMemory");
    testStream = pm::Stream(buffer, sizeof(buffer), pm::Stream::FlagRead, false);
        
    PM_LogInfo("Testing Common/Stream/PM_StreamGetSourceSize");
    PM_LogInfo("StreamSize: %zu", testStream.GetSize());

    PM_LogInfo("Testing Common/Stream/PM_StreamRead,PM_StreamPeek");
    PM_LogInfo("PeekedByte : %c", testStream.Peek<PM_Byte>());
    PM_LogInfo("ReadByte : %c", testStream.Peek<PM_Byte>());

    PM_LogInfo("Testing Common/Stream/PM_StreamRead,PM_StreamPeek,PM_StreamWrite");
    testStream.SetCursorPosition(0);
    PM_LogInfo("PeekedByte : %c", testStream.Peek<PM_Byte>());
    PM_LogInfo("ReadByte : %c", testStream.Peek<PM_Byte>());
    //PM_LogInfo("Writing Byte : %zu", PM_StreamWriteByte(&testStream, 49));

    

    PM_LogInfo("Finished test for Common/Stream");
    return 0;
}