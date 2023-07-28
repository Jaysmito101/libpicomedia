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
#define PICOMEDIA_IMAGE_FILE_FORMAT_PGM         0x0AA7
#define PICOMEDIA_IMAGE_FILE_FORMAT_PBM         0x0AA8
#define PICOMEDIA_IMAGE_FILE_FORMAT_PNM         0x0AA9
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
#define PICOMEDIA_IMAGE_CHANNEL_FORMAT_CMYK     0x0BA6 // for future use
#define PICOMEDIA_IMAGE_CHANNEL_FORMAT_YCBCR    0x0BA7 // for future use
#define PICOMEDIA_IMAGE_CHANNEL_FORMAT_YCCK     0x0BA8 // for future use
#define PICOMEDIA_IMAGE_CHANNEL_FORMAT_YCBCRA   0x0BA9 // for future use
#define PICOMEDIA_IMAGE_CHANNEL_FORMAT_YCCKA    0x0BAA // for future use
#define PICOMEDIA_IMAGE_CHANNEL_FORMAT_UNKNOWN  0x0BAB

#define PICOIMEDIA_IMAGE_DATA_TYPE_UINT8        0x0CA0
#define PICOIMEDIA_IMAGE_DATA_TYPE_UINT16       0x0CA1
#define PICOIMEDIA_IMAGE_DATA_TYPE_UINT32       0x0CA2
#define PICOIMEDIA_IMAGE_DATA_TYPE_UINT64       0x0CA3
#define PICOIMEDIA_IMAGE_DATA_TYPE_FLOAT32      0x0CA4
#define PICOIMEDIA_IMAGE_DATA_TYPE_FLOAT64      0x0CA5
#define PICOIMEDIA_IMAGE_DATA_TYPE_UNKNOWN      0x0CA6

struct PM_Image
{
    PM_Byte*   data;
    PM_Size    dataCapacity;
    PM_UInt32  width;
    PM_UInt32  height;
    PM_UInt32  channelFormat;
    PM_UInt32  dataType;
    PM_UInt8   numChannels;
    PM_UInt8   bitsPerChannel;
};
typedef struct PM_Image PM_Image;

void PICOMEDIA_API PM_ImageInit(PM_Image* image);
void PICOMEDIA_API PM_ImageDestroy(PM_Image* image);


#endif // LIBPICOMEDIA_IMAGE_BASE_H