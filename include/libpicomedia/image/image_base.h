#ifndef LIBPICOMEDIA_IMAGE_BASE_H
#define LIBPICOMEDIA_IMAGE_BASE_H

#include "libpicomedia/common/common.h"


#define PICOMEDIA_IMAGE_FILE_FORMAT_PNG         0x0AA0
#define PICOMEDIA_IMAGE_FILE_FORMAT_JPEG        0x0AA1
#define PICOMEDIA_IMAGE_FILE_FORMAT_BMP         0x0AA2
#define PICOMEDIA_IMAGE_FILE_FORMAT_TIFF        0x0AA3
#define PICOMEDIA_IMAGE_FILE_FORMAT_GIF         0x0AA4
#define PICOMEDIA_IMAGE_FILE_FORMAT_WEBP        0x0AA5
#define PICOMEDIA_IMAGE_FILE_FORMAT_PPM         0x0AA6
#define PICOMEDIA_IMAGE_FILE_FORMAT_HDR         0x0AAA
#define PICOMEDIA_IMAGE_FILE_FORMAT_EXR         0x0AAB
#define PICOMEDIA_IMAGE_FILE_FORMAT_ICO         0x0AAC
#define PICOMEDIA_IMAGE_FILE_FORMAT_TGA         0x0AAD
#define PICOMEDIA_IMAGE_FILE_FORMAT_JPEG2000    0x0AAE
#define PICOMEDIA_IMAGE_FILE_FORMAT_RAW         0x0AAF
#define PICOMEDIA_IMAGE_FILE_FORMAT_PSD         0x0AB0
#define PICOMEDIA_IMAGE_FILE_FORMAT_HEIF        0x0AB1
#define PICOMEDIA_IMAGE_FILE_FORMAT_CUR         0x0AB2
#define PICOMEDIA_IMAGE_FILE_FORMAT_DDS         0x0AB3
#define PICOMEDIA_IMAGE_FILE_FORMAT_PICT        0x0AB4
#define PICOMEDIA_IMAGE_FILE_FORMAT_SVG         0x0AB5
#define PICOMEDIA_IMAGE_FILE_FORMAT_AVIF        0x0AB6
#define PICOMEDIA_IMAGE_FILE_FORMAT_UNKNOWN     0x0AB7

#define PICOMEDIA_IMAGE_CHANNEL_FORMAT_RGB      0x0BA0
#define PICOMEDIA_IMAGE_CHANNEL_FORMAT_RGBA     0x0BA1
#define PICOMEDIA_IMAGE_CHANNEL_FORMAT_BGR      0x0BA2
#define PICOMEDIA_IMAGE_CHANNEL_FORMAT_BGRA     0x0BA3
#define PICOMEDIA_IMAGE_CHANNEL_FORMAT_GRAY     0x0BA4
#define PICOMEDIA_IMAGE_CHANNEL_FORMAT_GRAYA    0x0BA5
#define PICOMEDIA_IMAGE_CHANNEL_FORMAT_UNKNOWN  0x0BAB

#define PICOIMEDIA_IMAGE_DATA_TYPE_UINT8        0x0CA0
#define PICOIMEDIA_IMAGE_DATA_TYPE_UINT16       0x0CA1
#define PICOIMEDIA_IMAGE_DATA_TYPE_UINT32       0x0CA2
#define PICOIMEDIA_IMAGE_DATA_TYPE_UINT64       0x0CA3
#define PICOIMEDIA_IMAGE_DATA_TYPE_FLOAT32      0x0CA4
#define PICOIMEDIA_IMAGE_DATA_TYPE_FLOAT64      0x0CA5
#define PICOIMEDIA_IMAGE_DATA_TYPE_UNKNOWN      0x0CA6

/**
 * @file image_base.h
 * @brief This file contains the definition of the PM_Image struct and its associated typedef.
 */

/**
 * @brief A struct representing an image.
 */
struct PM_Image
{
    PM_Byte*   data;           /**< Pointer to the image data. */
    PM_Size    dataCapacity;   /**< The capacity of the image data buffer. */
    PM_Size    dataSize;       /**< The size of the image data in bytes. */
    PM_UInt32  width;          /**< The width of the image in pixels. */
    PM_UInt32  height;         /**< The height of the image in pixels. */
    PM_UInt32  channelFormat;  /**< The format of the image channels. */
    PM_UInt32  dataType;       /**< The data type of the image. */
    PM_UInt8   numChannels;    /**< The number of channels in the image. */
    PM_UInt8   bitsPerChannel; /**< The number of bits per channel in the image. */
};
 /** Typedef for PM_Image struct. */
typedef struct PM_Image PM_Image;

/**
 * Initializes a PM_Image struct with default values.
 * 
 * NOTE: This doesn't allocate memory for the image data buffer.
 *       To allocate memory for the image data buffer, use PM_ImageAllocate().
 *
 * @param image Pointer to a PM_Image struct to be initialized.
 */
void PICOMEDIA_API PM_ImageInit(PM_Image* image);

/**
 * @brief Destroys an image object and frees its memory if it was allocated, and sets the pointer to NULL, and initializes the image object with default values.
 * 
 * @param image Pointer to the image object to be destroyed.
 */
void PICOMEDIA_API PM_ImageDestroy(PM_Image* image);


/**
 * @brief Copies the contents of one PM_Image to another.
 * 
 * This function copies the contents of the source PM_Image to the destination PM_Image.
 * It only allocates new memory for the destination PM_Image if the destination PM_Image's data buffer is NULL or the destination PM_Image's data buffer is not large enough to hold the source PM_Image's data.
 * 
 * @param dest Pointer to the destination PM_Image.
 * @param src Pointer to the source PM_Image.
 * @return PM_TRUE if the copy was successful, PM_FALSE otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImageCopy(PM_Image* dest, const PM_Image* src);


/**
 * @brief Allocates memory for a PM_Image struct and initializes its properties.
 * 
 * @param image Pointer to a PM_Image struct to be allocated and initialized.
 * @param width Width of the image in pixels.
 * @param height Height of the image in pixels.
 * @param channelFormat Format of the image channels.
 * @param dataType Data type of the image.
 * @param numChannels Number of channels in the image.
 */
PM_Bool PICOMEDIA_API PM_ImageAllocate(PM_Image* image, PM_UInt32 width, PM_UInt32 height, PM_UInt32 channelFormat, PM_UInt32 dataType, PM_UInt8 numChannels);


/**
 * Returns the size of the data type for the given image data type.
 *
 * @param dataType The image data type.
 * @return The size of the data type in bytes.
 */
PM_Size PM_ImageGetDataTypeSize(PM_UInt32 dataType);


/**
 * @brief Converts an image file format to a string representation.
 * @param imageFileFormat The image file format to convert.
 * @return A string representation of the image file format.
 */
const PM_Char* PM_ImageFileFormatToString(PM_UInt32 imageFileFormat);


/**
 * @brief Converts an image channel format to a string representation.
 * @param channelFormat The image channel format to convert.
 * @return A string representation of the image channel format.
 */
const PM_Char* PM_ImageChannelFromatToString(PM_UInt32 channelFormat);


/**
 * @brief Converts an image data type to a string representation.
 * @param dataType The image data type to convert.
 * @return A string representation of the image data type.
 */
const PM_Char* PM_ImageDataTypeToString(PM_UInt32 dataType);


/**
 * @brief Gets the pixel value of an image at a specified location and channel.
 * 
 * @param image Pointer to the PM_Image struct representing the image.
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @param channel The channel of the pixel to retrieve.
 * @param pixelValue Pointer to a PM_Byte variable to store the pixel value. Ignored if NULL.
 * @return PM_Float64 The pixel value as a floating-point number.
 */
PM_Float64 PICOMEDIA_API PM_ImageGetPixelValue(const PM_Image* image, PM_UInt32 x, PM_UInt32 y, PM_UInt8 channel, PM_Byte* pixelValue);


/**
 * Sets the pixel value of an image at a specified position and channel.
 *
 * @param image Pointer to the PM_Image struct representing the image.
 * @param x The x-coordinate of the pixel to set.
 * @param y The y-coordinate of the pixel to set.
 * @param channel The channel of the pixel to set.
 * @param pixelValue The value to set the pixel to.
 *
 * @return PM_TRUE if the pixel value was successfully set, PM_FALSE otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImageSetPixelValue(PM_Image* image, PM_UInt32 x, PM_UInt32 y, PM_UInt8 channel, PM_Float64 pixelValue);

#endif // LIBPICOMEDIA_IMAGE_BASE_H
