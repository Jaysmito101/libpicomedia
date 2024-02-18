#ifndef PICOMEDIA_COMMON_STREAM_H
#define PICOMEDIA_COMMON_STREAM_H

#include "libpicomedia/common/common_base.h"

#define PICOMEDIA_STREAM_SOURCE_TYPE_FILE        0x00000001
#define PICOMEDIA_STREAM_SOURCE_TYPE_MEMORY      0x00000002
#define PICOMEDIA_STREAM_SOURCE_TYPE_NETWORK     0x00000004 // for future use
#define PICOMEDIA_STREAM_SOURCE_TYPE_UNKNOWN     0x00000008

#define PICOMEDIA_STREAM_FLAG_READ               0x00000001
#define PICOMEDIA_STREAM_FLAG_WRITE              0x00000002
#define PICOMEDIA_STREAM_FLAG_APPEND             0x00000004


/**
 * @brief Structure representing a stream of data.
 * 
 * This structure contains information about the source of the stream, such as a file or memory buffer,
 * as well as the current position of the stream's cursor and various flags and settings.
 */
struct PM_Stream
{
    FILE* fileSource;       /**< Pointer to the file source, if applicable. */
    PM_Byte* memorySource;  /**< Pointer to the memory source, if applicable. */
    PM_Size sourceSize;     /**< Size of the source data, in bytes. */
    PM_Int64 cursorPosition;/**< Current position of the stream's cursor. */
    PM_UInt32 sourceType;   /**< Type of the stream's source, such as file or memory. */
    PM_UInt8 flags;         /**< Flags indicating the stream's read/write/append permissions. */
    PM_Bool isInitialized;  /**< Flag indicating whether the stream has been initialized. */
    PM_Bool isSourceOwner;  /**< Flag indicating whether the stream is the owner of its source data. */
    PM_Bool requireReverse; /**< Flag indicating whether the stream's data should be reversed when reading if target endianess is different from host endianess. */
    PM_Size sizeForReverse; /**< Max size of data that will be reversed when reading if target endianess is different from host endianess. */
};

/** Typedef for PM_Stream struct. */
typedef struct PM_Stream PM_Stream;


/**
 * @brief Initializes a PM_Stream struct with default values.
 * 
 * This function initializes a PM_Stream struct with default values for all of its fields. This is useful
 * when creating a new stream from scratch, or when resetting an existing stream to its default state.
 * 
 * @param stream Pointer to the PM_Stream struct to initialize.
 */
void PICOMEDIA_API PM_StreamInit(PM_Stream* stream);

/**
 * @brief Initializes a PM_Stream struct from a file.
 * 
 * This function initializes a PM_Stream struct from a file on disk. The file is opened in the specified
 * mode, and the stream's cursor is set to the beginning of the file.
 * 
 * NOTE: This function internally calls PM_StreamInitFromFileDescriptor
 * 
 * @param stream Pointer to the PM_Stream struct to initialize.
 * @param fileName Name of the file to open.
 * @param flags Flags indicating the stream's read/write/append permissions.
 * @return PM_Bool PM_TRUE if the stream was successfully initialized, PM_FALSE otherwise.
*/
PM_Bool PICOMEDIA_API PM_StreamInitFromFile(PM_Stream* stream, const char* fileName, PM_UInt8 flags);

/**
 * @brief Initializes a PM_Stream struct from a file descriptor.
 * 
 * This function initializes a PM_Stream struct from a file descriptor. The file is opened in the specified
 * mode, and the stream's cursor is set to the beginning of the file.
 * 
 * @param stream Pointer to the PM_Stream struct to initialize.
 * @param file Pointer to the file descriptor to use.
 * @param flags Flags indicating the stream's read/write/append permissions.
 * @param isSourceOwner Flag indicating whether the stream is the owner of its source data.
 * @return PM_Bool PM_TRUE if the stream was successfully initialized, PM_FALSE otherwise. * 
*/
PM_Bool PICOMEDIA_API PM_StreamInitFromFileDescriptor(PM_Stream* stream, FILE* file, PM_UInt8 flags, PM_Bool isSourceOwner);

/**
 * @brief Initializes a PM_Stream struct from a memory buffer.
 * 
 * This function initializes a PM_Stream struct from a memory buffer. The stream's cursor is set to the
 * beginning of the buffer.
 * 
 * @param stream Pointer to the PM_Stream struct to initialize.
 * @param memory Pointer to the memory buffer to use.
 * @param size Size of the memory buffer, in bytes.
 * @param flags Flags indicating the stream's read/write/append permissions.
 * @param isSourceOwner Flag indicating whether the stream is the owner of its source data.
*/
PM_Bool PICOMEDIA_API PM_StreamInitFromMemory(PM_Stream* stream, PM_Byte* memory, PM_Size size, PM_UInt8 flags, PM_Bool isSourceOwner);

/**
 * @brief Initializes a PM_Stream struct from a network address.
 * 
 * This function initializes a PM_Stream struct from a network address. The stream's cursor is set to the
 * beginning of the buffer.
 * 
 * NOTE: This function is not yet implemented.
 * 
 * @param stream Pointer to the PM_Stream struct to initialize.
 * @param address Network address to use.
 * @param flags Flags indicating the stream's read/write/append permissions.
 * @return PM_Bool PM_TRUE if the stream was successfully initialized, PM_FALSE otherwise.
*/
PM_Bool PICOMEDIA_API PM_StreamInitFromNetwork(PM_Stream* stream, const char* address, PM_UInt8 flags);

/**
 * @brief Destroys a PM_Stream struct.
 * 
 * This function destroys a PM_Stream struct, freeing any resources that it may have allocated.
 * The resource are freed only if the stream is the owner of its source data.
 * 
 * @param stream Pointer to the PM_Stream struct to destroy.
 * @return PM_Bool PM_TRUE if the stream was successfully destroyed, PM_FALSE otherwise.
*/
void PICOMEDIA_API PM_StreamDestroy(PM_Stream* stream);

/**
 * @brief Reads data from a stream.
 * 
 * This function reads data from a stream into a buffer. The amount of data read is determined by the
 * size parameter. The stream's cursor is advanced by the amount of data read.
 * 
 * @param stream Pointer to the PM_Stream struct to read from.
 * @param buffer Pointer to the buffer to read into.
 * @param size Number of bytes to read.
 * @return PM_Size Number of bytes read.
*/
PM_Size PICOMEDIA_API PM_StreamRead(PM_Stream* stream, PM_Byte* buffer, PM_Size size);

/**
 * @brief Writes data to a stream.
 * 
 * This function writes data to a stream from a buffer. The amount of data written is determined by the
 * size parameter. The stream's cursor is advanced by the amount of data written.
 * 
 * @param stream Pointer to the PM_Stream struct to write to.
 * @param buffer Pointer to the buffer to write from.
 * @param size Number of bytes to write.
 * @return PM_Size Number of bytes written.
*/
PM_Size PICOMEDIA_API PM_StreamWrite(PM_Stream* stream, const PM_Byte* buffer, PM_Size size);

/**
 * @brief Peeks at data from a stream.
 * 
 * This function peeks at data from a stream into a buffer. The amount of data peeked is determined by the
 * size parameter. The stream's cursor is not advanced.
 * 
 * NOTE: This function internally calls PM_StreamRead
 * 
 * @param stream Pointer to the PM_Stream struct to peek from.
 * @param buffer Pointer to the buffer to peek into.
 * @param size Number of bytes to peek.
 * @return PM_Size Number of bytes peeked.
*/
PM_Size PICOMEDIA_API PM_StreamPeek(PM_Stream* stream, PM_Byte* buffer, PM_Size size);

/**
 * @brief Gets the current position of the stream's cursor.
 * 
 * This function gets the current position of the stream's cursor.
 * 
 * @param stream Pointer to the PM_Stream struct to get the cursor position from.
 * @return PM_Size Current position of the stream's cursor.
*/
PM_Size PICOMEDIA_API PM_StreamGetCursorPosition(PM_Stream* stream);

/**
 * @brief Sets the current position of the stream's cursor.
 * 
 * This function sets the current position of the stream's cursor to the specified position.
 * 
 * @param stream Pointer to the PM_Stream struct to set the cursor position for.
 * @param position The position to set the cursor to.
 * @return PM_Size The new position of the stream's cursor.
*/
PM_Size PICOMEDIA_API PM_StreamSetCursorPosition(PM_Stream* stream, PM_Size position);

/**
 * @brief Gets the size of the source data of a stream.
 * 
 * This function gets the size of the source data of a stream. If the stream does not own its source data,
 * the function returns 0.
 * 
 * @param stream Pointer to the PM_Stream struct to get the source size from.
 * @return PM_Size Size of the source data of the stream.
*/
PM_Size PICOMEDIA_API PM_StreamGetSourceSize(PM_Stream* stream);

/**
 * @brief Sets whether the stream requires reverse playback support.
 * 
 * @param stream The stream to set the reverse playback requirement for.
 * @param requireReverse Whether the stream requires reverse playback support.
 */
void PICOMEDIA_API PM_StreamSetRequireReverse(PM_Stream* stream, PM_Bool requireReverse);

/**
 * @brief Macro that adds read, write, and peek functions for a given type to a PM_Stream struct.
 * 
 * This macro defines a set of functions that can be used to read, write, and peek at values of a given type
 * from a PM_Stream struct. The type is specified as an argument to the macro, and the resulting functions
 * are named PM_StreamRead<type>, PM_StreamWrite<type>, and PM_StreamPeek<type>, respectively.
 * 
 * @param type The type to add read, write, and peek functions for.
*/
#define PICOMEDIA_STREAM_ADD_TYPE_FUNCTIONS_SOURCE(type) \
    static PM_##type PM_StreamRead##type(PM_Stream* stream) \
    { \
        PM_##type value; \
        PM_StreamRead(stream, (PM_Byte*)&value, sizeof(PM_##type)); \
        return value; \
    } \
    \
    static PM_Bool PM_StreamWrite##type(PM_Stream* stream, PM_##type value) \
    { \
        return PM_StreamWrite(stream, (PM_Byte*)&value, sizeof(PM_##type)) == sizeof(PM_##type); \
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