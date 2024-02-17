#include <stdio.h>

#include "libpicomedia/libpicomedia.h"

void test_write_gzip(const PM_Char* filename, const PM_UInt8* deflateData, PM_Size size, PM_Size uncompressedSize, PM_UInt32 crc) 
{
    PM_Stream stream = {0};

    PM_StreamInitFromFile(&stream, filename, PICOMEDIA_STREAM_FLAG_WRITE);

    static const PM_UInt8 gzipHeader[] = {
        0x1F, 0x8B
    };

    PM_StreamWrite(&stream, gzipHeader, sizeof(gzipHeader));

    static const PM_UInt8 deflateHeader[] = {
        0x08
    };

    PM_StreamWrite(&stream, deflateHeader, sizeof(deflateHeader));

    PM_StreamWriteUInt8(&stream, 0x00); // Flags

    PM_StreamWriteUInt32(&stream, 0x00); // Modification time

    PM_StreamWriteUInt8(&stream, 0x00); // Extra flags

    PM_StreamWriteUInt8(&stream, 0x00); // OS

    PM_StreamWrite(&stream, deflateData, size);

    PM_StreamWriteUInt32(&stream, crc);

    PM_StreamWriteUInt32(&stream, (PM_UInt32)uncompressedSize); // Uncompressed size

    PM_StreamDestroy(&stream);
}


int main()
{

    PM_LogInfo("Initializing sandbox...");

    
    static const PM_UInt8 buffer[] = {
        0xF3, 0x48, 0xCD, 0xC9, 0xC9, 0x57, 0x08, 0xCF, 0x2F, 0xCA, 0x49, 0x51, 0xE4, 0xE5, 0x72, 0x24, 
        0x0B, 0x00, 0x00
    };
    static const PM_Size buffer_size = sizeof(buffer);

    static const PM_UInt8 dummyData[] = "Hello World!\nAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
    static const PM_Size dummyDataSize = sizeof(dummyData);

    test_write_gzip("test.txt.gz", buffer, buffer_size, dummyDataSize, PM_CRC32(dummyData, dummyDataSize, 0));


    PM_LogInfo("Shutting down sandbox...");
    return 0;
}