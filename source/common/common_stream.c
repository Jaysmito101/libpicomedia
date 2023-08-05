#include "libpicomedia/common/stream.h"

// -----------------------------------------------------------------------------------------------

void PM_StreamInit(PM_Stream* stream)
{
    PM_Assert(stream != NULL);
    
    stream->fileSource = NULL;
    stream->memorySource = NULL;
    stream->sourceSize = 0;
    stream->cursorPosition = 0;
    stream->flags = 0;
    stream->sourceType = PICOMEDIA_STREAM_SOURCE_TYPE_UNKNOWN;
    stream->isInitialized = false;
    stream->isSourceOwner = false;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_StreamInitFromFileDescriptor(PM_Stream* stream, FILE* file, PM_UInt8 flags, PM_Bool isSourceOwner)
{
    PM_Assert(stream != NULL);
    PM_Assert(file != NULL);

    PM_StreamInit(stream);

    stream->fileSource = file;
    stream->sourceType = PICOMEDIA_STREAM_SOURCE_TYPE_FILE;
    stream->flags = flags;
    stream->isSourceOwner = isSourceOwner;
    stream->isInitialized = true;
    
    // get size of file
    fseek(file, 0, SEEK_END); // seek to end of file 
    stream->sourceSize = ftell(file); // get current file pointer
    // fseek(file, 0, SEEK_SET); // seek back to beginning of file
    PM_StreamSetCursorPosition(stream, 0);

    
    return true;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_StreamInitFromFile(PM_Stream* stream, const char* fileName, PM_UInt8 flags)
{
    PM_Assert(stream != NULL);
    
    const PM_Byte* fileOpenMode = (flags & PICOMEDIA_STREAM_FLAG_WRITE) ? (flags & PICOMEDIA_STREAM_FLAG_READ ? "rb+" : "wb+") : (flags & PICOMEDIA_STREAM_FLAG_WRITE ? "a+" : "rb");

    FILE* file = fopen(fileName, fileOpenMode);
    PM_Assert(file != NULL);
    
    return PM_StreamInitFromFileDescriptor(stream, file, flags, true);
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_StreamInitFromMemory(PM_Stream* stream, PM_Byte* memory, PM_Size size, PM_UInt8 flags, PM_Bool isSourceOwner)
{
    PM_Assert(stream != NULL);
    PM_Assert(memory != NULL);

    PM_StreamInit(stream);
    stream->memorySource = memory;
    stream->sourceSize = size;
    stream->sourceType = PICOMEDIA_STREAM_SOURCE_TYPE_MEMORY;
    stream->flags = flags;
    stream->isSourceOwner = isSourceOwner;
    stream->isInitialized = true;

    return true;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_StreamInitFromNetwork(PM_Stream* stream, const char* address, PM_UInt8 flags)
{
    (void)stream; (void)address; (void)flags;

    PM_LogError("PM_StreamInitFromNetwork is not yet supported!");

    return false;
}

// -----------------------------------------------------------------------------------------------

void PM_StreamDestroy(PM_Stream* stream)
{
    PM_Assert(stream != NULL);

    if (stream->isSourceOwner && stream->isInitialized)
    {
        if(stream->sourceType == PICOMEDIA_STREAM_SOURCE_TYPE_FILE && stream->fileSource)
            fclose(stream->fileSource);
        else if (stream->sourceType == PICOMEDIA_STREAM_SOURCE_TYPE_MEMORY && stream->memorySource)
            PM_Free(stream->memorySource);
        else
            PM_LogError("Invalid Stream Source Type %u", stream->sourceType);
    }

    PM_StreamInit(stream);
}

// -----------------------------------------------------------------------------------------------

PM_Size PM_StreamRead(PM_Stream* stream, PM_Byte* buffer, PM_Size size)
{
    PM_Assert(stream != NULL);
    PM_Assert(buffer != NULL);
    PM_Assert(stream->flags & PICOMEDIA_STREAM_FLAG_READ && !(stream->flags & PICOMEDIA_STREAM_FLAG_WRITE || stream->flags & PICOMEDIA_STREAM_FLAG_APPEND));

    if (stream->sourceType == PICOMEDIA_STREAM_SOURCE_TYPE_FILE)
    {
        PM_Size read = fread(buffer, 1, size, stream->fileSource);
        stream->cursorPosition += read;
        return read;
    }
    else if (stream->sourceType == PICOMEDIA_STREAM_SOURCE_TYPE_MEMORY)
    {
        PM_Size bytesToRead = size;
        if (stream->cursorPosition + size > stream->sourceSize)
            bytesToRead = stream->sourceSize - stream->cursorPosition;
        memcpy(buffer, stream->memorySource + stream->cursorPosition, bytesToRead);
        stream->cursorPosition += bytesToRead;
        return bytesToRead;
    }
    else
    {
        PM_LogError("Invalid Stream Source Type %u", stream->sourceType);
        return 0;
    }
}

// -----------------------------------------------------------------------------------------------

PM_Size PM_StreamWrite(PM_Stream* stream, const PM_Byte* buffer, PM_Size size)
{
    PM_Assert(stream != NULL);
    PM_Assert(buffer != NULL);
    PM_Assert((stream->flags & PICOMEDIA_STREAM_FLAG_WRITE || stream->flags & PICOMEDIA_STREAM_FLAG_APPEND) && !(stream->flags & PICOMEDIA_STREAM_FLAG_READ));
    PM_Assert((stream->cursorPosition + size <= stream->sourceSize) || (stream->sourceType == PICOMEDIA_STREAM_SOURCE_TYPE_FILE));

    if (stream->sourceType == PICOMEDIA_STREAM_SOURCE_TYPE_FILE)
    {
        PM_Size written = fwrite(buffer, 1, size, stream->fileSource);
        stream->cursorPosition += written;
        return written;
    }
    else if (stream->sourceType == PICOMEDIA_STREAM_SOURCE_TYPE_MEMORY)
    {
        memcpy(stream->memorySource + stream->cursorPosition, buffer, size);
        stream->cursorPosition += size;
        return size;
    }
    else
    {
        PM_LogError("Invalid Stream Source Type %u", stream->sourceType);
        return 0;
    }
}

// -----------------------------------------------------------------------------------------------

PM_Size PM_StreamPeek(PM_Stream* stream, PM_Byte* buffer, PM_Size size)
{
    PM_Assert(stream != NULL);

    PM_Size bytesRead = PM_StreamRead(stream, buffer, size);
    
    stream->cursorPosition -= bytesRead;
    if (stream->cursorPosition < 0)
        stream->cursorPosition = 0;

    PM_StreamSetCursorPosition(stream, stream->cursorPosition);    

    return bytesRead;
}

// -----------------------------------------------------------------------------------------------

PM_Size PM_StreamGetCursorPosition(PM_Stream* stream)
{
    PM_Assert(stream != NULL);

    return stream->cursorPosition;
}

// -----------------------------------------------------------------------------------------------

PM_Size PM_StreamSetCursorPosition(PM_Stream* stream, PM_Size position)
{
    PM_Assert(stream != NULL);

    if (position > stream->sourceSize)
        position = stream->sourceSize;
    
    stream->cursorPosition = position;
    long offset = (long)stream->cursorPosition;

    if (stream->sourceType == PICOMEDIA_STREAM_SOURCE_TYPE_FILE)
        fseek(stream->fileSource, offset, SEEK_SET);
    
    return stream->cursorPosition;
}

// -----------------------------------------------------------------------------------------------

PM_Size PM_StreamGetSourceSize(PM_Stream* stream)
{
    PM_Assert(stream != NULL);

    return stream->sourceSize;
}

// -----------------------------------------------------------------------------------------------
