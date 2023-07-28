#ifndef PICOMEDIA_COMMON_STREAM_H
#define PICOMEDIA_COMMON_STREAM_H

#include "libpicomedia/common/common_base.h"

#define PICOMEDIA_STREAM_SOURCE_TYPE_FILE        0x00000001
#define PICOMEDIA_STREAM_SOURCE_TYPE_MEMORY      0x00000002
#define PICOMEDIA_STREAM_SOURCE_TYPE_NETWORK     0x00000004 // for future use
#define PICOMEDIA_STREAM_SOURCE_TYPE_UNKNOWN     0x00000008

#define PICOMEDIA_STREAM_FLAG_READ               0b00000001
#define PICOMEDIA_STREAM_FLAG_WRITE              0b00000010
#define PICOMEDIA_STREAM_FLAG_APPEND             0b00000100

struct PM_Stream
{
    FILE* fileSource;
    PM_Byte* memorySource;
    PM_Size sourceSize;
    PM_Size cursorPosition;
    PM_UInt32 sourceType;
    PM_UInt8 flags;
    PM_Bool isInitialized;    
    PM_Bool isSourceOwner;
};
typedef struct PM_Stream PM_Stream;

void PICOMEDIA_API PM_StreamInit(PM_Stream* stream);
PM_Bool PICOMEDIA_API PM_StreamInitFromFile(PM_Stream* stream, const char* fileName, PM_UInt8 flags);
PM_Bool PICOMEDIA_API PM_StreamInitFromFileDescriptor(PM_Stream* stream, FILE* file, PM_UInt8 flags, PM_Bool isSourceOwner);
PM_Bool PICOMEDIA_API PM_StreamInitFromMemory(PM_Stream* stream, PM_Byte* memory, PM_Size size, PM_UInt8 flags, PM_Bool isSourceOwner);
PM_Bool PICOMEDIA_API PM_StreamInitFromNetwork(PM_Stream* stream, const char* address, PM_UInt8 flags);
void PICOMEDIA_API PM_StreamDestroy(PM_Stream* stream);

PM_Size PICOMEDIA_API PM_StreamRead(PM_Stream* stream, PM_Byte* buffer, PM_Size size);
PM_Size PICOMEDIA_API PM_StreamWrite(PM_Stream* stream, PM_Byte* buffer, PM_Size size);
PM_Size PICOMEDIA_API PM_StreamPeek(PM_Stream* stream, PM_Byte* buffer, PM_Size size);

PM_Size PICOMEDIA_API PM_StreamGetCursorPosition(PM_Stream* stream);
PM_Size PICOMEDIA_API PM_StreamSetCursorPosition(PM_Stream* stream, PM_Size position);
PM_Size PICOMEDIA_API PM_StreamGetSourceSize(PM_Stream* stream);

#define PICOMEDIA_STREAM_ADD_TYPE_FUNCTIONS_SOURCE(type) \
    static PM_##type PM_StreamRead##type(PM_Stream* stream) \
    { \
        PM_##type value; \
        PM_StreamRead(stream, (PM_Byte*)&value, sizeof(PM_##type)); \
        return value; \
    } \
    \
    static PM_Size PM_StreamWrite##type(PM_Stream* stream, PM_##type value) \
    { \
        return PM_StreamWrite(stream, (PM_Byte*)&value, sizeof(PM_##type)); \
    } \
    \
    static PM_##type PM_StreamPeek##type(PM_Stream* stream) \
    { \
        PM_##type value; \
        PM_StreamPeek(stream, (PM_Byte*)&value, sizeof(PM_##type)); \
        return value; \
    }

PICOMEDIA_STREAM_ADD_TYPE_FUNCTIONS_SOURCE(Int8)
PICOMEDIA_STREAM_ADD_TYPE_FUNCTIONS_SOURCE(Int16)
PICOMEDIA_STREAM_ADD_TYPE_FUNCTIONS_SOURCE(Int32)
PICOMEDIA_STREAM_ADD_TYPE_FUNCTIONS_SOURCE(Int64)
PICOMEDIA_STREAM_ADD_TYPE_FUNCTIONS_SOURCE(UInt8)
PICOMEDIA_STREAM_ADD_TYPE_FUNCTIONS_SOURCE(UInt16)
PICOMEDIA_STREAM_ADD_TYPE_FUNCTIONS_SOURCE(UInt32)
PICOMEDIA_STREAM_ADD_TYPE_FUNCTIONS_SOURCE(UInt64)
PICOMEDIA_STREAM_ADD_TYPE_FUNCTIONS_SOURCE(Float32)
PICOMEDIA_STREAM_ADD_TYPE_FUNCTIONS_SOURCE(Float64)
PICOMEDIA_STREAM_ADD_TYPE_FUNCTIONS_SOURCE(Byte)


    




#endif // PICOMEDIA_COMMON_STREAM_H