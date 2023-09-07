#include "libpicomedia/image/image_base.h"

// -----------------------------------------------------------------------------------------------

static PM_Bool PM__ImageEnsureSize(PM_Image* image, PM_Size requiredSize)
{
    PM_Assert(image != NULL);
    PM_Assert(requiredSize > 0);

    if ( (image->data != NULL) && (image->dataCapacity < requiredSize) )
    {
        PM_Free(image->data);
        image->data = NULL;
    }

    if (image->data == NULL)
    {
        image->data = (PM_Byte*)PM_Malloc(requiredSize);
        image->dataCapacity = requiredSize;
        return image->data != NULL;
    }

    return PM_TRUE;
}

// -----------------------------------------------------------------------------------------------

void PM_ImageInit(PM_Image* image)
{
    PM_Assert(image != NULL);

    image->width = 0;
    image->height = 0;
    image->data = NULL;
    image->dataCapacity = 0;
    image->numChannels = 0;
    image->bitsPerChannel = 0;
    image->channelFormat = PICOMEDIA_IMAGE_CHANNEL_FORMAT_UNKNOWN;
    image->dataType = PICOIMEDIA_IMAGE_DATA_TYPE_UNKNOWN;
    image->dataSize = 0;
}

// -----------------------------------------------------------------------------------------------

void PM_ImageDestroy(PM_Image* image)
{
    PM_Assert(image != NULL);

    if (image->data != NULL)
    {
        PM_Free(image->data);
        image->data = NULL;
    }
    PM_ImageInit(image);
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImageCopy(PM_Image* dest, const PM_Image* src)
{
    PM_Assert(dest != NULL);
    PM_Assert(src != NULL);

    if (! PM__ImageEnsureSize(dest, src->dataSize) )
    {
        PM_LogError("PM_ImageCopy: Failed to allocate memory for destination image.");
        return PM_FALSE;
    }

    PM_Memcpy(dest->data, src->data, src->dataSize);

    dest->width = src->width;
    dest->height = src->height;
    dest->numChannels = src->numChannels;
    dest->bitsPerChannel = src->bitsPerChannel;
    dest->channelFormat = src->channelFormat;
    dest->dataType = src->dataType;
    dest->dataSize = src->dataSize;

    return PM_TRUE;
}

// -----------------------------------------------------------------------------------------------

PM_Size PM_ImageGetDataTypeSize(PM_UInt32 dataType)
{
    switch (dataType)
    {
        case PICOIMEDIA_IMAGE_DATA_TYPE_UINT8   : return 1;
        case PICOIMEDIA_IMAGE_DATA_TYPE_UINT16  : return 2;
        case PICOIMEDIA_IMAGE_DATA_TYPE_UINT32  : return 4;
        case PICOIMEDIA_IMAGE_DATA_TYPE_UINT64  : return 8;
        case PICOIMEDIA_IMAGE_DATA_TYPE_FLOAT32 : return 4;
        case PICOIMEDIA_IMAGE_DATA_TYPE_FLOAT64 : return 8;
        case PICOIMEDIA_IMAGE_DATA_TYPE_UNKNOWN : return 0;
        default                                 : return 0;
    }
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImageAllocate(PM_Image* image, PM_UInt32 width, PM_UInt32 height, PM_UInt32 channelFormat, PM_UInt32 dataType, PM_UInt8 numChannels)
{
    PM_Assert( image != NULL );
    PM_Assert( (width > 0) && (height > 0) );
    PM_Assert( numChannels > 0 );

    PM_Size requiredSize = width * height * numChannels * PM_ImageGetDataTypeSize(dataType);
    
    if (! PM__ImageEnsureSize(image, requiredSize) )
    {
        PM_LogError("PM_ImageAllocate: Failed to allocate %zu bytes for image data.", requiredSize);
        return PM_FALSE; // Allocation failed
    }

    image->width = width;
    image->height = height;
    image->numChannels = numChannels;
    image->channelFormat = channelFormat;
    image->dataType = dataType;
    image->dataSize = requiredSize;
    image->bitsPerChannel = (PM_UInt8)PM_ImageGetDataTypeSize(dataType) * 8;

    return PM_TRUE;
}

// -----------------------------------------------------------------------------------------------

const PM_Char* PM_ImageFileFormatToString(PM_UInt32 imageFileFormat)
{
    switch (imageFileFormat)
    {
        case PICOMEDIA_IMAGE_FILE_FORMAT_PNG         : return "PNG";
        case PICOMEDIA_IMAGE_FILE_FORMAT_JPEG        : return "JPEG";
        case PICOMEDIA_IMAGE_FILE_FORMAT_BMP         : return "BMP";
        case PICOMEDIA_IMAGE_FILE_FORMAT_TIFF        : return "TIFF";
        case PICOMEDIA_IMAGE_FILE_FORMAT_GIF         : return "GIF";
        case PICOMEDIA_IMAGE_FILE_FORMAT_WEBP        : return "WEBP";
        case PICOMEDIA_IMAGE_FILE_FORMAT_PPM         : return "PPM";
        case PICOMEDIA_IMAGE_FILE_FORMAT_HDR         : return "HDR";
        case PICOMEDIA_IMAGE_FILE_FORMAT_EXR         : return "EXR";
        case PICOMEDIA_IMAGE_FILE_FORMAT_ICO         : return "ICO";
        case PICOMEDIA_IMAGE_FILE_FORMAT_TGA         : return "TGA";
        case PICOMEDIA_IMAGE_FILE_FORMAT_JPEG2000    : return "JPEG2000";
        case PICOMEDIA_IMAGE_FILE_FORMAT_RAW         : return "RAW";
        case PICOMEDIA_IMAGE_FILE_FORMAT_PSD         : return "PSD";
        case PICOMEDIA_IMAGE_FILE_FORMAT_HEIF        : return "HEIF";
        case PICOMEDIA_IMAGE_FILE_FORMAT_CUR         : return "CUR";
        case PICOMEDIA_IMAGE_FILE_FORMAT_DDS         : return "DDS";
        case PICOMEDIA_IMAGE_FILE_FORMAT_PICT        : return "PICT";
        case PICOMEDIA_IMAGE_FILE_FORMAT_SVG         : return "SVG";
        case PICOMEDIA_IMAGE_FILE_FORMAT_AVIF        : return "AVIF";
        default                                      : return "UNKNOWN";
    }
}

// -----------------------------------------------------------------------------------------------

const PM_Char* PM_ImageChannelFromatToString(PM_UInt32 channelFormat)
{
    switch (channelFormat)
    {
        case PICOMEDIA_IMAGE_CHANNEL_FORMAT_RGB     : return "RGB";
        case PICOMEDIA_IMAGE_CHANNEL_FORMAT_RGBA    : return "RGBA";
        case PICOMEDIA_IMAGE_CHANNEL_FORMAT_BGR     : return "BGR";
        case PICOMEDIA_IMAGE_CHANNEL_FORMAT_BGRA    : return "BGRA";
        case PICOMEDIA_IMAGE_CHANNEL_FORMAT_GRAY    : return "GRAY";
        case PICOMEDIA_IMAGE_CHANNEL_FORMAT_GRAYA   : return "GRAYA";
        default                                     : return "UNKNOWN";
    }
}

// -----------------------------------------------------------------------------------------------

const PM_Char* PM_ImageDataTypeToString(PM_UInt32 dataType)
{
    switch (dataType)
    {
        case PICOIMEDIA_IMAGE_DATA_TYPE_UINT8   : return "UINT8";     
        case PICOIMEDIA_IMAGE_DATA_TYPE_UINT16  : return "UINT16";    
        case PICOIMEDIA_IMAGE_DATA_TYPE_UINT32  : return "UINT32";    
        case PICOIMEDIA_IMAGE_DATA_TYPE_UINT64  : return "UINT64";    
        case PICOIMEDIA_IMAGE_DATA_TYPE_FLOAT32 : return "FLOAT32";   
        case PICOIMEDIA_IMAGE_DATA_TYPE_FLOAT64 : return "FLOAT64";   
        default                                 : return "UNKNOWN";   
    }
}

// -----------------------------------------------------------------------------------------------

PM_Float64 PM_ImageGetPixelValue(const PM_Image* image, PM_UInt32 x, PM_UInt32 y, PM_UInt8 channel, PM_Byte* pixelValue)
{
    PM_Assert(image != NULL);
    PM_Assert( (x < image->width) && (y < image->height) );
    PM_Assert( channel < image->numChannels );

    PM_Size bytesPerChannel = image->bitsPerChannel / 8;
    PM_Size bytesPerPixel = image->numChannels * bytesPerChannel;
    PM_Size pixelOffset = (y * image->width + x) * bytesPerPixel + channel * bytesPerChannel;
    PM_Byte* pixel = image->data + pixelOffset;

    if (pixelValue != NULL)
    {
        memcpy(pixelValue, pixel, bytesPerChannel);
    }

    switch (image->dataType)
    {
        case PICOIMEDIA_IMAGE_DATA_TYPE_UINT8   : return (PM_Float64)(*((PM_UInt8*)pixel) / (PM_Float64)((1 << 8) - 1));
        case PICOIMEDIA_IMAGE_DATA_TYPE_UINT16  : return (PM_Float64)(*((PM_UInt16*)pixel) / (PM_Float64)((1 << 16) - 1));
        case PICOIMEDIA_IMAGE_DATA_TYPE_UINT32  : return (PM_Float64)(*((PM_UInt32*)pixel) / (PM_Float64)((1ll << 32) - 1));
        case PICOIMEDIA_IMAGE_DATA_TYPE_UINT64  : return (PM_Float64)(*((PM_UInt64*)pixel) / 18446744073709551615.0);
        case PICOIMEDIA_IMAGE_DATA_TYPE_FLOAT32 : return (PM_Float64)(*((PM_Float32*)pixel));
        case PICOIMEDIA_IMAGE_DATA_TYPE_FLOAT64 : return (PM_Float64)(*((PM_Float64*)pixel));
        default                                 : return 0.0;
    }

}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_ImageSetPixelValue(PM_Image* image, PM_UInt32 x, PM_UInt32 y, PM_UInt8 channel, PM_Float64 pixelValue)
{
    PM_Assert(image != NULL);
    PM_Assert( (x < image->width) && (y < image->height) );
    PM_Assert( channel < image->numChannels );

    PM_Size bytesPerChannel = image->bitsPerChannel / 8;
    PM_Size bytesPerPixel = image->numChannels * bytesPerChannel;
    PM_Size pixelOffset = (y * image->width + x) * bytesPerPixel + channel * bytesPerChannel;
    PM_Byte* pixel = image->data + pixelOffset;

    switch (image->dataType)
    {
        case PICOIMEDIA_IMAGE_DATA_TYPE_UINT8   : *((PM_UInt8*)pixel) = (PM_UInt8)(pixelValue * (PM_Float64)((1 << 8) - 1)); break;
        case PICOIMEDIA_IMAGE_DATA_TYPE_UINT16  : *((PM_UInt16*)pixel) = (PM_UInt16)(pixelValue * (PM_Float64)((1 << 16) - 1)); break;
        case PICOIMEDIA_IMAGE_DATA_TYPE_UINT32  : *((PM_UInt32*)pixel) = (PM_UInt32)(pixelValue * (PM_Float64)((1ll << 32) - 1)); break;
        case PICOIMEDIA_IMAGE_DATA_TYPE_UINT64  : *((PM_UInt64*)pixel) = (PM_UInt64)(pixelValue * 18446744073709551615.0); break;
        case PICOIMEDIA_IMAGE_DATA_TYPE_FLOAT32 : *((PM_Float32*)pixel) = (PM_Float32)pixelValue; break;
        case PICOIMEDIA_IMAGE_DATA_TYPE_FLOAT64 : *((PM_Float64*)pixel) = (PM_Float64)pixelValue; break;
        default                                 : return PM_FALSE;
    }

    return PM_TRUE;
}

// -----------------------------------------------------------------------------------------------