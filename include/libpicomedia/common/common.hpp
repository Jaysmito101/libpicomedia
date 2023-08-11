#pragma once

extern "C"
{
#include "libpicomedia/common/common.h"
}

#include <string>
#include <stdexcept>

namespace picomedia
{

    class Stream
    {
    public:
        enum Flags
        {
            FlagRead = PICOMEDIA_STREAM_FLAG_READ,
            FlagWrite = PICOMEDIA_STREAM_FLAG_WRITE,
            FlagAppend = PICOMEDIA_STREAM_FLAG_APPEND
        };

        enum SourceType
        {
            SourceTypeFile = PICOMEDIA_STREAM_SOURCE_TYPE_FILE,
            SourceTypeMemory = PICOMEDIA_STREAM_SOURCE_TYPE_MEMORY,
            SourceTypeNetwork = PICOMEDIA_STREAM_SOURCE_TYPE_NETWORK            
        };

    public:
        Stream() 
        {
            PM_StreamInit(&m_Stream);
        }

        Stream(const std::string& path, PM_UInt8 flags = FlagRead)
        {
            if(!PM_StreamInitFromFile(&m_Stream, path.c_str(), flags))
                throw std::runtime_error("Failed to open file: " + path);
        }

        Stream(PM_Byte* data, PM_Size size, PM_UInt8 flags = FlagRead, PM_Bool isOwner = true)
        {
            if(!PM_StreamInitFromMemory(&m_Stream, data, size, flags, isOwner))
                throw std::runtime_error("Failed to open memory stream");
        }

        virtual ~Stream()
        {
            PM_StreamDestroy(&m_Stream);
        }

        inline PM_Size ReadI(PM_Byte* buffer, PM_Size size)
        {
            return PM_StreamRead(&m_Stream, buffer, size);
        }

        inline PM_Size WriteI(const PM_Byte* buffer, PM_Size size)
        {
            return PM_StreamWrite(&m_Stream, buffer, size);
        }

        inline PM_Size Peek(PM_Byte* buffer, PM_Size size)
        {
            return PM_StreamPeek(&m_Stream, buffer, size);
        }

        inline PM_Size GetCursorPosition()
        {
            return PM_StreamGetCursorPosition(&m_Stream);
        }

        inline PM_Size GetSize()
        {
            return PM_StreamGetSourceSize(&m_Stream);
        }

        inline PM_Size SetCursorPosition(PM_Size position)
        {
            return PM_StreamSetCursorPosition(&m_Stream, position);
        }
        
        inline PM_Stream& GetInternalHandle()
        {
            return m_Stream;
        }

        inline PM_Stream* GetInternalHandlePtr()
        {
            return &m_Stream;
        }

        template<typename T>
        inline T Read()
        {
            T value = T();
            ReadI((PM_Byte*)&value, sizeof(T));
            return value;
        }

        template<typename T>
        inline PM_Size Write(T value)
        {
            return WriteI((PM_Byte*)&value, sizeof(T));
        }

        template<typename T>
        inline T Peek()
        {
            T value = T();
            Peek((PM_Byte*)&value, sizeof(T));
            return value;
        }

    private:
        PM_Stream m_Stream = {0};
    };


    namespace utils 
    {

        inline PM_Bool CharIsWhiteSpace(char ch)
        {
            return PM_CharIsWhiteSpace(ch);
        }

        inline PM_Int64 ReadASCIIIntegerFromStream(Stream& stream)
        {
            return PM_ReadASCIIIntegerFromStream(stream.GetInternalHandlePtr());
        }

        inline PM_Bool IsBigEndian() 
        {
            return PM_IsBigEndian();
        }

    }

}