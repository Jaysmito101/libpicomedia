#ifndef PICOMEDIA_IMAGE_BMP_H
#define PICOMEDIA_IMAGE_BMP_H

#include "libpicomedia/image/image_base.h"

/**
 * @file bmp.h
 * @brief This file contains the definition of the BMP header structure.
 */
 
/**
 * @brief The BMP header structure.
 */
struct PM_BMPHeader
{
    PM_UInt16 signature; /**< The signature of the BMP file. */
    PM_UInt32 fileSize; /**< The size of the BMP file in bytes. */
    PM_UInt32 reserved; /**< Reserved field. */
    PM_UInt32 dataOffset; /**< The offset to the start of the pixel data. */
};
typedef struct PM_BMPHeader PM_BMPHeader;

/**
 * @brief Struct representing the BMP info header.
 * 
 */
struct PM_BMPInfoHeader
{
    PM_UInt32 headerSize; /**< The size of the BMP info header. */
    PM_Int32 width; /**< The width of the BMP image in pixels. */
    PM_Int32 height; /**< The height of the BMP image in pixels. */
    PM_UInt16 planes; /**< The number of color planes. */
    PM_UInt16 bitsPerPixel; /**< The number of bits per pixel. */
    PM_UInt32 compression; /**< The type of compression used. */
    PM_UInt32 imageSize; /**< The size of the image data in bytes. */
    PM_Int32 xPixelsPerMeter; /**< The horizontal resolution of the image in pixels per meter. */
    PM_Int32 yPixelsPerMeter; /**< The vertical resolution of the image in pixels per meter. */
    PM_UInt32 colorsUsed; /**< The number of colors used in the image. */
    PM_UInt32 colorsImportant; /**< The number of important colors used in the image. */
};
typedef struct PM_BMPInfoHeader PM_BMPInfoHeader;

/**
 * @brief Struct representing a single color table item in a BMP image.
 * 
 */
struct PM_BMPColorTableItem
{
    PM_UInt8 blue; /**< The blue component of the color. */
    PM_UInt8 green; /**< The green component of the color. */
    PM_UInt8 red; /**< The red component of the color. */
    PM_UInt8 reserved; /**< Reserved for future use. */
};
typedef struct PM_BMPColorTableItem PM_BMPColorTableItem;

/**
 * @brief The PM_BMPContext struct represents a BMP image context.
 */
struct PM_BMPContext
{
    PM_BMPHeader header; /**< The BMP header. */
    PM_BMPInfoHeader infoHeader; /**< The BMP info header. */
    PM_BMPColorTableItem* colorTable; /**< The BMP color table. */
    PM_Size colorTableCapacity; /**< The BMP color table capacity. */
    PM_Byte* imageData; /**< The BMP image data. */
    PM_Size imageDataCapacity; /**< The BMP image data capacity. */
};
typedef struct PM_BMPContext PM_BMPContext;

/**
 * Initializes a BMP context.
 *
 * @param context A pointer to a PM_BMPContext struct.
 */
void PICOMEDIA_API PM_ImageBMPContextInit(PM_BMPContext* context);

/**
 * @brief Destroys a BMP context.
 * 
 * This function destroys a BMP context and frees any memory allocated for it.
 * 
 * @param context Pointer to the BMP context to be destroyed.
 */
void PICOMEDIA_API PM_ImageBMPContextDestroy(PM_BMPContext* context);

/**
 * Initializes a BMP color table item.
 *
 * @param item The BMP color table item to be initialized.
 */
void PICOMEDIA_API PM_ImageBMPColorTableItemInit(PM_BMPColorTableItem* item);

/**
 * @brief Prints the given BMP color table item.
 * 
 * @param item Pointer to the BMP color table item to be printed.
 */
void PICOMEDIA_API PM_ImageBMPColorTableItemPrint(const PM_BMPColorTableItem* item);

/**
 * Initializes a BMP header structure with default values.
 *
 * @param header A pointer to a PM_BMPHeader structure to be initialized.
 */
void PICOMEDIA_API PM_ImageBMPHeaderInit(PM_BMPHeader* header);

/**
 * Prints the BMP header information of an image.
 *
 * @param header The BMP header of the image.
 */
void PICOMEDIA_API PM_ImageBMPHeaderPrint(const PM_BMPHeader* header);

/**
 * Initializes a BMP info header.
 *
 * @param infoHeader Pointer to a PM_BMPInfoHeader struct to be initialized.
 */
void PICOMEDIA_API PM_ImageBMPInfoHeaderInit(PM_BMPInfoHeader* infoHeader);

/**
 * Prints the information header of a BMP image.
 *
 * @param infoHeader A pointer to the BMP information header to be printed.
 */
void PICOMEDIA_API PM_ImageBMPInfoHeaderPrint(const PM_BMPInfoHeader* infoHeader);


/**
 * @brief Detects if the given stream contains a BMP image.
 * 
 * @param stream Pointer to a PM_Stream struct representing the stream to check.
 * @return PM_Bool Returns PM_TRUE if the stream contains a BMP image, PM_FALSE otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImageBMPDetect(PM_Stream* stream);


/**
 * Detects if the given data or file is a BMP image.
 * 
 * @param data A pointer to the data to be checked if it is a BMP image.
 * @param size The size of the data to be checked.
 * @param filename The name of the file to be checked if it is a BMP image.
 * 
 * @return Returns true if the given data or file is a BMP image, otherwise false.
 */
PM_Bool PICOMEDIA_API PM_ImageBMPDetectFroMemory(PM_Byte* data, PM_Size size);


/**
 * Detects if the specified file is a BMP image.
 *
 * @param filename The path to the file to be checked.
 * @return PM_TRUE if the file is a BMP image, PM_FALSE otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImageBMPDetectFromFile(const char* filename);


/**
 * Reads the BMP header from the given stream.
 *
 * @param stream The stream to read from.
 * @param header Pointer to the BMP header structure to fill.
 * @return True if the header was successfully read, false otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImageBMPReadHeader(PM_Stream* stream, PM_BMPHeader* header);

/**
 * Reads the BMP info header from the given stream.
 *
 * @param stream The stream to read from.
 * @param infoHeader Pointer to the BMP info header structure to fill.
 * @return True if the info header was successfully read, false otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImageBMPReadInfoHeader(PM_Stream* stream, PM_BMPInfoHeader* infoHeader);

/**
 * Reads the BMP color table from the given stream.
 *
 * @param stream The stream to read from.
 * @param infoHeader Pointer to the BMP info header structure.
 * @param colorTable Pointer to the BMP color table array to fill.
 * @param colorTableCapacity Pointer to the variable storing the capacity of the color table array.
 * @return True if the color table was successfully read, false otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImageBMPReadColorTable(PM_Stream* stream, const PM_BMPInfoHeader* infoHeader, PM_BMPColorTableItem** colorTable, PM_Size* colorTableCapacity);

/**
 * Reads the BMP image data from the given stream.
 *
 * @param stream The stream to read from.
 * @param header Pointer to the BMP header structure.
 * @param imageData Pointer to the variable storing the image data.
 * @param imageDataSize Pointer to the variable storing the size of the image data.
 * @return True if the image data was successfully read, false otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImageBMPReadImageData(PM_Stream* stream, const PM_BMPHeader* header, PM_Byte** imageData, PM_Size* imageDataSize);

/**
 * Decodes the BMP image using the provided context.
 *
 * @param context Pointer to the BMP decoding context.
 * @param image Pointer to the image structure to fill.
 * @return True if the image was successfully decoded, false otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImageBMPDecode(const PM_BMPContext* context, PM_Image* image);

/**
 * Reads the BMP image from the given stream.
 *
 * @param stream The stream to read from.
 * @param image Pointer to the image structure to fill.
 * @return True if the image was successfully read, false otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImageBMPRead(PM_Stream* stream, PM_Image* image);

/**
 * Reads the BMP image from the specified file.
 *
 * @param filePath The path to the BMP file.
 * @param image Pointer to the image structure to fill.
 * @return True if the image was successfully read, false otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImageBMPReadFromFile(const PM_Byte* filePath, PM_Image* image);

/**
 * Reads the BMP image from the given memory buffer.
 *
 * @param data Pointer to the memory buffer containing the BMP data.
 * @param dataSize The size of the BMP data in bytes.
 * @param image Pointer to the image structure to fill.
 * @return True if the image was successfully read, false otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImageBMPReadFromMemory(PM_Byte* data, PM_Size dataSize, PM_Image* image);



/**
 * Writes the BMP header to the specified stream.
 *
 * @param stream The stream to write the header to.
 * @param header The BMP header to write.
 * @return Returns true if the header was successfully written, false otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImageBMPWriteHeader(PM_Stream* stream, const PM_BMPHeader* header);

/**
 * Writes the BMP info header to the specified stream.
 *
 * @param stream The stream to write the info header to.
 * @param infoHeader The BMP info header to write.
 * @return Returns true if the info header was successfully written, false otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImageBMPWriteInfoHeader(PM_Stream* stream, const PM_BMPInfoHeader* infoHeader);

/**
 * Writes the BMP color table to the specified stream.
 *
 * @param stream The stream to write the color table to.
 * @param colorTable The BMP color table to write.
 * @param colorTableSize The size of the color table.
 * @return Returns true if the color table was successfully written, false otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImageBMPWriteColorTable(PM_Stream* stream, const PM_BMPColorTableItem* colorTable, PM_Size colorTableSize);

/**
 * Encodes the specified image in BMP format using the provided context.
 *
 * @param image The image to encode.
 * @param context The BMP encoding context.
 * @return Returns true if the image was successfully encoded, false otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImageBMPEncode(const PM_Image* image, PM_BMPContext* context);

/**
 * Writes the specified image to the specified stream in BMP format.
 *
 * @param image The image to write.
 * @param stream The stream to write the image to.
 * @return Returns true if the image was successfully written, false otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImageBMPWrite(const PM_Image* image, PM_Stream* stream);

/**
 * Writes the specified image to the specified file path in BMP format.
 *
 * @param image The image to write.
 * @param filePath The file path to write the image to.
 * @return Returns true if the image was successfully written, false otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImageBMPWriteToFile(const PM_Image* image, const PM_Byte* filePath);

/**
 * Writes the specified image to the specified memory buffer in BMP format.
 *
 * @param image The image to write.
 * @param data The memory buffer to write the image to.
 * @param dataSize The size of the memory buffer. On success, this will be updated with the actual size of the written data.
 * @param maxDataSize The maximum size of the memory buffer.
 * @return Returns true if the image was successfully written, false otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImageBMPWriteToMemory(const PM_Image* image, PM_Byte* data, PM_Size* dataSize, PM_Size maxDataSize);


#endif // PICOMEDIA_IMAGE_BMP_H
