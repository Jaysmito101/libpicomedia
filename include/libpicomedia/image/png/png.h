#ifndef PICOMEDIA_IMAGE_PNG_H
#define PICOMEDIA_IMAGE_PNG_H

#include "libpicomedia/image/image_base.h"


/** @file png.h
 * @brief Functions for reading and writing PNG images.
*/


/**
 * @brief Structure representing the header information of a PNG image.
 * 
 * This corresponds to the IHDR chunk of a PNG file.
 * 
 * This structure contains the width, height, bit depth, color type, compression method,
 * filter method, and interlace method of a PNG image.
 */
struct PM_PNGHeader
{
    PM_UInt32 width;            /**< Width of the image in pixels. */
    PM_UInt32 height;           /**< Height of the image in pixels. */
    PM_UInt8 bitDepth;          /**< Bit depth of the image. */
    PM_UInt8 colorType;         /**< Color type of the image. */
    PM_UInt8 compressionMethod; /**< Compression method used in the image. */
    PM_UInt8 filterMethod;      /**< Filter method used in the image. */
    PM_UInt8 interlaceMethod;   /**< Interlace method used in the image. */
};
typedef struct PM_PNGHeader PM_PNGHeader;

/**
 * @brief Structure representing a PNG palette.
 * 
 * This corresponds to the PLTE chunk of a PNG file.
 * 
 * This structure contains an array of color data and the size of the palette.
 */
struct PM_PNGPallette
{
    PM_UInt8 data[256][3]; /**< Array of color data. */
    PM_Size size; /**< Number of colors in the palette. */
};
typedef struct PM_PNGPallette PM_PNGPallette;

/**
 * @brief Union representing the transparency data for PNG images.
 * 
 * This union can be used to store transparency information for PNG images.
 * It provides different ways to represent transparency based on the color mode.
 * 
 * For grayscale images, the transparency is represented by a 16-bit unsigned integer.
 * For RGB images, the transparency is represented by an array of three 16-bit unsigned integers.
 * For indexed color images, the transparency is represented by an array of 256 8-bit unsigned integers.
 * 
 * The transparency data can be accessed and modified using the appropriate member variables.
 * 
 * This corresponds to the tRNS chunk of a PNG file.
 */
union PM_PNGTransparency
{
    PM_UInt16 gray;         /**< Transparency for grayscale images */
    PM_UInt16 rgb[3];       /**< Transparency for RGB images */
    PM_UInt8 alpha[256];    /**< Transparency for indexed color images */
};
typedef union PM_PNGTransparency PM_PNGTransparency;

/**
 * @brief Structure representing the chromaticity values for a PNG image.
 * 
 * This structure stores the chromaticity values for a PNG image, including the white point
 * coordinates and the red, green, and blue primary chromaticity coordinates.
 */
struct PM_PNGChrm
{
    PM_Float32 whitePointX; /**< The X coordinate of the white point */
    PM_Float32 whitePointY; /**< The Y coordinate of the white point */
    PM_Float32 redX; /**< The X coordinate of the red primary chromaticity */
    PM_Float32 redY; /**< The Y coordinate of the red primary chromaticity */
    PM_Float32 greenX; /**< The X coordinate of the green primary chromaticity */
    PM_Float32 greenY; /**< The Y coordinate of the green primary chromaticity */
    PM_Float32 blueX; /**< The X coordinate of the blue primary chromaticity */
    PM_Float32 blueY; /**< The Y coordinate of the blue primary chromaticity */
};
typedef struct PM_PNGChrm PM_PNGChrm;

/**
 * @brief Structure representing an ICC profile in a PNG image.
 * 
 * This structure contains information about an ICC profile used in a PNG image.
 * It includes the name of the profile, the compression method used, and the compressed data.
 * 
 * NOTE: This structure is not yet implemented.
 */
struct PM_PNGICCProfile
{
    PM_Char name[80];           /**< Name of the ICC profile. */
    PM_UInt8 compressionMethod; /**< Compression method used for the profile data. */
    PM_UInt8* compressedData;   /**< Pointer to the compressed profile data. */
    PM_Size compressedDataSize; /**< Size of the compressed profile data. */
};
typedef struct PM_PNGICCProfile PM_PNGICCProfile;

/**
 * @brief Structure representing a PNG text chunk.
 * 
 * This corresponds to the tEXt, zTXt, and iTXt chunks of a PNG file. (for now, only tEXt is implemented)
 */
struct PM_PNGTextChunk
{
    PM_Char keyword[80]; /**< The keyword associated with the text chunk. */
    PM_Char* text; /**< The text data of the chunk. */
};
typedef struct PM_PNGTextChunk PM_PNGTextChunk;

/**
 * @brief Represents a PNG time chunk containing date and time information.
 * 
 * This corresponds to the tIME chunk of a PNG file.
 */
struct PM_PNGTimeChunk
{
    PM_UInt16 year;    /**< The year. */
    PM_UInt8 month;    /**< The month. */
    PM_UInt8 day;      /**< The day. */
    PM_UInt8 hour;     /**< The hour. */
    PM_UInt8 minute;   /**< The minute. */
    PM_UInt8 second;   /**< The second. */
};
typedef struct PM_PNGTimeChunk PM_PNGTimeChunk;

/**
 * @brief Structure representing the context of a PNG image.
 * 
 * This structure contains various components of a PNG image, such as the header, palette,
 * transparency, chromaticity, gamma, ICC profile, rendering intent, text chunks, EXIF data,
 * time chunk, and the actual image data.
 * 
 * As of now (18-02-2024) only these (a small subset of the actual PNG Specification) are implemented.
 */
struct PM_PNGContext
{
    PM_PNGHeader* header;                   /**< Pointer to the PNG header */
    PM_PNGPallette* palette;                 /**< Pointer to the PNG palette */
    PM_PNGTransparency* transparency;        /**< Pointer to the PNG transparency */
    PM_PNGChrm* chrm;                        /**< Pointer to the PNG chromaticity */
    PM_Float32 gamma;                        /**< Gamma value of the PNG image */
    PM_PNGICCProfile* iccProfile;            /**< Pointer to the PNG ICC profile */
    PM_UInt8 sRGBRenderingIntent;            /**< Rendering intent for sRGB color space */
    PM_PNGTextChunk* textChunks;             /**< Pointer to the PNG text chunks */
    PM_Size textChunkCount;                  /**< Number of text chunks */
    PM_UInt8* exifData;                      /**< Pointer to the EXIF data */
    PM_Size exifDataSize;                    /**< Size of the EXIF data */
    PM_PNGTimeChunk* timeChunk;              /**< Pointer to the PNG time chunk */
    PM_UInt8* rawData;                       /**< Pointer to the image data */
    PM_Size dataSize;                        /**< Size of the image data */
};
typedef struct PM_PNGContext PM_PNGContext;


// Utility Functions

/**
 * Initializes a PNG header structure with default values.
 *
 * @param header Pointer to the PM_PNGHeader struct to initialize.
 */
void PICOMEDIA_API PM_ImagePNGHeaderInit(PM_PNGHeader* header);

/**
 * Prints the contents of a PNG header structure to the log.
 * 
 * @param header Pointer to the PM_PNGHeader struct to print.
*/
void PICOMEDIA_API PM_ImagePNGHeaderPrint(const PM_PNGHeader* header);

/**
 * Checks if the given PNG header is valid.
 * 
 * @param header Pointer to the PM_PNGHeader struct to check.
 * @return PM_Bool Returns PM_TRUE if the header is valid, PM_FALSE otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImagePNGHeaderIsValid(const PM_PNGHeader* header);

/**
 * @file png.h
 * @brief This file contains the declarations of functions related to PNG image handling.
 */

/**
 * @brief Initializes a PNG palette structure.
 * 
 * @param palette The PNG palette structure to initialize.
 */
void PICOMEDIA_API PM_ImagePNGPalletteInit(PM_PNGPallette* palette);

/**
 * @brief Prints the contents of a PNG palette structure.
 * 
 * @param palette The PNG palette structure to print.
 */
void PICOMEDIA_API PM_ImagePNGPallettePrint(const PM_PNGPallette* palette);

/**
 * @brief Initializes a PNG transparency structure.
 * 
 * @param transparency The PNG transparency structure to initialize.
 */
void PICOMEDIA_API PM_ImagePNGTransparencyInit(PM_PNGTransparency* transparency);

/**
 * @brief Prints the contents of a PNG transparency structure.
 * 
 * @param transparency The PNG transparency structure to print.
 * @param colorType The color type of the image.
 */
void PICOMEDIA_API PM_ImagePNGTransparencyPrint(const PM_PNGTransparency* transparency, PM_UInt8 colorType);

/**
 * @brief Initializes a PNG chromaticity structure.
 * 
 * @param chrm The PNG chromaticity structure to initialize.
 */
void PICOMEDIA_API PM_ImagePNGChrmInit(PM_PNGChrm* chrm);

/**
 * @brief Prints the contents of a PNG chromaticity structure.
 * 
 * @param chrm The PNG chromaticity structure to print.
 */
void PICOMEDIA_API PM_ImagePNGChrmPrint(const PM_PNGChrm* chrm);

/**
 * @brief Initializes a PNG ICC profile structure.
 * 
 * @param iccProfile The PNG ICC profile structure to initialize.
 */
void PICOMEDIA_API PM_ImagePNGICCProfileInit(PM_PNGICCProfile* iccProfile);

/**
 * @brief Destroys a PNG ICC profile structure.
 * 
 * @param iccProfile The PNG ICC profile structure to destroy.
 */
void PICOMEDIA_API PM_ImagePNGICCProfileDestroy(PM_PNGICCProfile* iccProfile);

/**
 * @brief Prints the contents of a PNG ICC profile structure.
 * 
 * @param iccProfile The PNG ICC profile structure to print.
 */
void PICOMEDIA_API PM_ImagePNGICCProfilePrint(const PM_PNGICCProfile* iccProfile);

/**
 * @brief Initializes a PNG text chunk structure.
 * 
 * @param textChunk The PNG text chunk structure to initialize.
 */
void PICOMEDIA_API PM_ImagePNGTextChunkInit(PM_PNGTextChunk* textChunk);

/**
 * @brief Destroys a PNG text chunk structure.
 * 
 * @param textChunk The PNG text chunk structure to destroy.
 */
void PICOMEDIA_API PM_ImagePNGTextChunkDestroy(PM_PNGTextChunk* textChunk);

/**
 * @brief Prints the contents of a PNG text chunk structure.
 * 
 * @param textChunk The PNG text chunk structure to print.
 */
void PICOMEDIA_API PM_ImagePNGTextChunkPrint(const PM_PNGTextChunk* textChunk);

/**
 * @brief Initializes a PNG time chunk structure.
 * 
 * @param timeChunk The PNG time chunk structure to initialize.
 */
void PICOMEDIA_API PM_ImagePNGTimeChunkInit(PM_PNGTimeChunk* timeChunk);

/**
 * @brief Prints the contents of a PNG time chunk structure.
 * 
 * @param timeChunk The PNG time chunk structure to print.
 */
void PICOMEDIA_API PM_ImagePNGTimeChunkPrint(const PM_PNGTimeChunk* timeChunk);

/**
 * @brief Initializes a PNG context structure.
 * 
 * @param context The PNG context structure to initialize.
 */
void PICOMEDIA_API PM_ImagePNGContextInit(PM_PNGContext* context);

/**
 * @brief Destroys a PNG context structure.
 * 
 * @param context The PNG context structure to destroy.
 */
void PICOMEDIA_API PM_ImagePNGContextDestroy(PM_PNGContext* context);

/**
 * @brief Prints the contents of a PNG context structure.
 * 
 * @param context The PNG context structure to print.
 */
void PICOMEDIA_API PM_ImagePNGContextPrint(const PM_PNGContext* context);



// Detection Function


/**
 * @brief Detects if the given stream or memory data or file is in PNG format.
 * 
 * @param stream Pointer to a PM_Stream struct that contains the stream to check.
 * @return PM_Bool Returns PM_TRUE if the stream is in PNG format, PM_FALSE otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImagePNGDetect(PM_Stream* stream);


/**
 * @brief Detects if the given memory data is in PNG format.
 * 
 * @param data Pointer to the memory buffer containing the data to check.
 * @param dataSize The size of the memory buffer.
 * @return PM_Bool Returns PM_TRUE if the memory data is in PNG format, PM_FALSE otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImagePNGDetectFromMemory(PM_Byte* data, PM_Size dataSize);


/**
 * @brief Detects if the given file is in PNG format.
 * 
 * @param filePath The path to the file to check.
 * @return PM_Bool Returns PM_TRUE if the file is in PNG format, PM_FALSE otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImagePNGDetectFromFile(const char* filePath);



// Reading Functions


/**
 * @brief Reads a PNG image from a stream.
 *
 * This function reads a PNG image from the specified stream and stores it in the provided PM_Image structure.
 *
 * @param stream The stream from which to read the PNG image.
 * @param image The PM_Image structure to store the read image.
 * @return PM_Bool Returns PM_TRUE if the PNG image was successfully read, PM_FALSE otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImagePNGRead(PM_Stream* stream, PM_Image* image);

/**
 * @brief Reads a PNG image from a file.
 *
 * This function reads a PNG image from the specified file path and stores it in the provided PM_Image structure.
 *
 * @param filePath The file path of the PNG image to read.
 * @param image The PM_Image structure to store the read image.
 * @return PM_Bool Returns PM_TRUE if the PNG image was successfully read, PM_FALSE otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImagePNGReadFromFile(const PM_Byte* filePath, PM_Image* image);

/**
 * @brief Reads a PNG image from memory.
 *
 * This function reads a PNG image from the specified memory buffer and stores it in the provided PM_Image structure.
 *
 * @param data The memory buffer containing the PNG image data.
 * @param dataSize The size of the PNG image data in bytes.
 * @param image The PM_Image structure to store the read image.
 * @return PM_Bool Returns PM_TRUE if the PNG image was successfully read, PM_FALSE otherwise.
 */
PM_Bool PICOMEDIA_API PM_ImagePNGReadFromMemory(PM_Byte* data, PM_Size dataSize, PM_Image* image);






#endif // PICOMEDIA_IMAGE_PNG_H