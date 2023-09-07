#pragma once

#include "libpicomedia/image/image.h"

#include "libpicomedia/common/common.hpp"

#include <string>

namespace picomedia 
{
    namespace image 
    {

        enum ImageFormat
        {
            ImageFormatPNG          = PICOMEDIA_IMAGE_FILE_FORMAT_PNG,
            ImageFormatJPEG         = PICOMEDIA_IMAGE_FILE_FORMAT_JPEG,
            ImageFormatBMP          = PICOMEDIA_IMAGE_FILE_FORMAT_BMP,
            ImageFormatTIFF         = PICOMEDIA_IMAGE_FILE_FORMAT_TIFF,
            ImageFormatGIF          = PICOMEDIA_IMAGE_FILE_FORMAT_GIF,
            ImageFormatWEBP         = PICOMEDIA_IMAGE_FILE_FORMAT_WEBP,
            ImageFormatPPM          = PICOMEDIA_IMAGE_FILE_FORMAT_PPM,
            ImageFormatHDR          = PICOMEDIA_IMAGE_FILE_FORMAT_HDR,
            ImageFormatEXR          = PICOMEDIA_IMAGE_FILE_FORMAT_EXR,
            ImageFormatICO          = PICOMEDIA_IMAGE_FILE_FORMAT_ICO,
            ImageFormatTGA          = PICOMEDIA_IMAGE_FILE_FORMAT_TGA,
            ImageFormatJPEG2000     = PICOMEDIA_IMAGE_FILE_FORMAT_JPEG2000,
            ImageFormatRAW          = PICOMEDIA_IMAGE_FILE_FORMAT_RAW,
            ImageFormatPSD          = PICOMEDIA_IMAGE_FILE_FORMAT_PSD,
            ImageFormatHEIF         = PICOMEDIA_IMAGE_FILE_FORMAT_HEIF,
            ImageFormatCUR          = PICOMEDIA_IMAGE_FILE_FORMAT_CUR,
            ImageFormatDDS          = PICOMEDIA_IMAGE_FILE_FORMAT_DDS,
            ImageFormatPICT         = PICOMEDIA_IMAGE_FILE_FORMAT_PICT,
            ImageFormatSVG          = PICOMEDIA_IMAGE_FILE_FORMAT_SVG,
            ImageFormatAVIF         = PICOMEDIA_IMAGE_FILE_FORMAT_AVIF,
            ImageFormatUnknown      = PICOMEDIA_IMAGE_FILE_FORMAT_UNKNOWN
        };

        enum ChannelFormat
        {
            ChannelFromatRGB        = PICOMEDIA_IMAGE_CHANNEL_FORMAT_RGB,
            ChannelFromatRGBA       = PICOMEDIA_IMAGE_CHANNEL_FORMAT_RGBA,
            ChannelFromatBGR        = PICOMEDIA_IMAGE_CHANNEL_FORMAT_BGR,
            ChannelFromatBGRA       = PICOMEDIA_IMAGE_CHANNEL_FORMAT_BGRA,
            ChannelFromatGRAY       = PICOMEDIA_IMAGE_CHANNEL_FORMAT_GRAY,
            ChannelFromatGRAYA      = PICOMEDIA_IMAGE_CHANNEL_FORMAT_GRAYA,
            ChannelFromatUnknown    = PICOMEDIA_IMAGE_CHANNEL_FORMAT_UNKNOWN
        };

        enum DataType
        {
            DataTypeUInt8      = PICOIMEDIA_IMAGE_DATA_TYPE_UINT8,
            DataTypeUInt16     = PICOIMEDIA_IMAGE_DATA_TYPE_UINT16,
            DataTypeUInt32     = PICOIMEDIA_IMAGE_DATA_TYPE_UINT32,
            DataTypeUInt64     = PICOIMEDIA_IMAGE_DATA_TYPE_UINT64,
            DataTypeFloat32    = PICOIMEDIA_IMAGE_DATA_TYPE_FLOAT32,
            DataTypeFloat64    = PICOIMEDIA_IMAGE_DATA_TYPE_FLOAT64,
            DataTypeUnknown    = PICOIMEDIA_IMAGE_DATA_TYPE_UNKNOWN
        };

        inline std::string ToString(ChannelFormat cf)
        {
            return std::string(PM_ImageChannelFromatToString(cf));
        }

        inline std::string ToString(DataType dt)
        {
            return std::string(PM_ImageDataTypeToString(dt));
        }

        inline PM_Size GetDataTypeSize(DataType dt)
        {
            return PM_ImageGetDataTypeSize(dt);
        }

        class Image
        {
        public:

        Image()
        {
            PM_ImageInit(&m_Image);
        }

        ~Image()
        {
            PM_ImageDestroy(&m_Image);
        }

        Image(const Image& other)
        {
            PM_ImageCopy(&m_Image, &other.m_Image);
        }

        Image& operator=(const Image& other)
        {
            PM_ImageCopy(&m_Image, &other.m_Image);
            return *this;
        }

        inline PM_Bool CopyTo(Image& other) const
        {
            return PM_ImageCopy(&other.m_Image, &m_Image);
        }

        inline PM_Bool CopyFrom(const Image& other)
        {
            return PM_ImageCopy(&m_Image, &other.m_Image);
        }

        inline PM_Bool Allocate(PM_UInt32 width, PM_UInt32 height, ChannelFormat channelFormat, DataType daraType, PM_UInt8 numChannels)
        {
            return PM_ImageAllocate(&m_Image, width, height, channelFormat, daraType, numChannels);
        }

        inline PM_Float64 GetPixelValue(PM_UInt32 x, PM_UInt32 y, PM_UInt8 channel, PM_Byte* data = nullptr) const
        {
            return PM_ImageGetPixelValue(&m_Image, x, y, channel, data);
        }

        inline PM_Bool SetPixelValue(PM_UInt32 x, PM_UInt32 y, PM_UInt8 channel, PM_Float64 value)
        {
            return PM_ImageSetPixelValue(&m_Image, x, y, channel, value);
        }

        inline PM_Image& GetImage()
        {
            return m_Image;
        }

        inline const PM_Image& GetImage() const
        {
            return m_Image;
        }

        inline PM_Image* GetImagePtr()
        {
            return &m_Image;
        }

        inline const PM_Image* GetImagePtr() const
        {
            return &m_Image;
        }

        protected:
            PM_Image m_Image = {};
        };

    }
}

