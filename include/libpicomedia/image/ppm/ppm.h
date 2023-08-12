#ifndef PICOMEDIA_IMAGE_PPM_H
#define PICOMEDIA_IMAGE_PPM_H

#include "libpicomedia/image/image_base.h"

#define PICOMEDIA_PPM_FORMAT_P3      0x01
#define PICOMEDIA_PPM_FORMAT_P6      0x02
#define PICOMEDIA_PPM_FORMAT_UNKNOWN 0x03

// Utility Functions

/**
 * Returns a string representation of the PPM image format.
 *
 * @param ppmFormat The PPM image format.
 * @return The string representation of the PPM image format.
 */
const PM_Char* PM_ImagePPMFormatToString(PM_UInt32 ppmFormat);

// Detection Function


/**
 * @brief Detects if the given stream or memory data or file is in PPM format.
 * 
 * @param stream Pointer to a PM_Stream struct that contains the stream to check.
 * @return PM_UInt32 Returns PICOMEDIA_PPM_FORMAT_P3 if the stream is in P3 format, PICOMEDIA_PPM_FORMAT_P6 if the stream is in P6 format, otherwise PICOMEDIA_PPM_FORMAT_UNKNOWN.
 */
PM_UInt32 PICOMEDIA_API PM_ImagePPMDetect(PM_Stream* stream);


/**
 * @brief Detects if the given memory data is in PPM format.
 * 
 * @param data Pointer to a PM_Byte array that contains the memory data to check.
 * @param dataSize Size of the memory data in bytes.
 * @return PM_UInt32 Returns PICOMEDIA_PPM_FORMAT_P3 if the memory data is in P3 format, PICOMEDIA_PPM_FORMAT_P6 if the memory data is in P6 format, otherwise PICOMEDIA_PPM_FORMAT_UNKNOWN.
 */
PM_UInt32 PICOMEDIA_API PM_ImagePPMDetectFromMemory(PM_Byte* data, PM_Size dataSize);


/**
 * @brief Detects if the given file is in PPM format.
 * 
 * @param filePath Path to the file to check.
 * @return PM_UInt32 Returns PICOMEDIA_PPM_FORMAT_P3 if the file is in P3 format, PICOMEDIA_PPM_FORMAT_P6 if the file is in P6 format, otherwise PICOMEDIA_PPM_FORMAT_UNKNOWN.
 */
PM_UInt32 PICOMEDIA_API PM_ImagePPMDetectFromFile(const char* filePath);



// Reading Functions

/**
 * @brief Reads a PPM image file in P6 format from a stream.
 * 
 * @param stream The stream to read the image from.
 * @param image The image structure to store the read image data.
 * @return PM_Bool Returns PM_TRUE if the image was read successfully, PM_FALSE otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImagePPMReadP6(PM_Stream* stream, PM_Image* image);

/**
 * @brief Reads a PPM image file in P3 format from a stream.
 * 
 * @param stream The stream to read the image from.
 * @param image The image structure to store the read image data.
 * @return PM_Bool Returns PM_TRUE if the image was read successfully, PM_FALSE otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImagePPMReadP3(PM_Stream* stream, PM_Image* image);

/**
 * @brief Reads a PPM image file from a stream.
 * 
 * This function automatically detects the format of the PPM file and calls the appropriate
 * function to read the image data.
 * 
 * @param stream The stream to read the image from.
 * @param image The image structure to store the read image data.
 * @return PM_Bool Returns PM_TRUE if the image was read successfully, PM_FALSE otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImagePPMRead(PM_Stream* stream, PM_Image* image);

/**
 * @brief Reads a PPM image file from a memory buffer.
 * 
 * @param data The memory buffer containing the image data.
 * @param dataSize The size of the memory buffer.
 * @param image The image structure to store the read image data.
 * @return PM_Bool Returns PM_TRUE if the image was read successfully, PM_FALSE otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImagePPMReadFromMemory(PM_Byte* data, PM_Size dataSize, PM_Image* image);

/**
 * @brief Reads a PPM image file from a file on disk.
 * 
 * @param filePath The path to the file to read.
 * @param image The image structure to store the read image data.
 * @return PM_Bool Returns PM_TRUE if the image was read successfully, PM_FALSE otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImagePPMReadFromFile(const char* filePath, PM_Image* image);

// Writing Functions

/**
 * @brief Writes a PPM image file in P6 format to a stream.
 * 
 * @param stream The stream to write the image file to.
 * @param image The image to write to the file.
 * @return PM_Bool Returns PM_TRUE if the image was successfully written to the stream, PM_FALSE otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImagePPMWriteP6(const PM_Image* image, PM_Stream* stream);

/**
 * @brief Writes a PPM image file in P3 format to a stream.
 * 
 * @param stream The stream to write the image file to.
 * @param image The image to write to the file.
 * @return PM_Bool Returns PM_TRUE if the image was successfully written to the stream, PM_FALSE otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImagePPMWriteP3(const PM_Image* image, PM_Stream* stream);

/**
 * @brief Writes a PPM image file to a stream.
 * 
 * @param ppmFormat The format of the PPM file to write (P3 or P6).
 * @param stream The stream to write the image file to.
 * @param image The image to write to the file.
 * @return PM_Bool Returns PM_TRUE if the image was successfully written to the stream, PM_FALSE otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImagePPMWrite(PM_UInt32 ppmFormat, const PM_Image* image, PM_Stream* stream);

/**
 * @brief Writes a PPM image file to a file on disk.
 * 
 * @param ppmFormat The format of the PPM file to write (P3 or P6).
 * @param filePath The path to the file to write the image to.
 * @param image The image to write to the file.
 * @return PM_Bool Returns PM_TRUE if the image was successfully written to the file, PM_FALSE otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImagePPMWriteToFile(PM_UInt32 ppmFormat, const PM_Image* image, const PM_Byte* filePath);

/**
 * @brief Writes a PPM image file to a memory buffer.
 * 
 * @param ppmFormat The format of the PPM file to write (P3 or P6).
 * @param image The image to write to the memory buffer.
 * @param data The memory buffer to write the image to.
 * @param dataSize A pointer to the size of the memory buffer. On return, this will be set to the size of the data written.
 * @param maxDataSize The maximum size of the memory buffer.
 * @return PM_Bool Returns PM_TRUE if the image was successfully written to the memory buffer, PM_FALSE otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImagePPMWriteToMemory(PM_UInt32 ppmFormat, const PM_Image* image, PM_Byte* data, PM_Size* dataSize, PM_Size maxDataSize);

#endif // PICOMEDIA_IMAGE_PPM_H