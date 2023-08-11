#pragma once

#include "libpicomedia/common/common.hpp"
#include "libpicomedia/image/image.hpp"
#include "libpicomedia/image/ppm/ppm.h"

#include <string>

namespace picomedia 
{
    namespace image 
    {

        enum PPMFormat
        {
            PPMFormatP3         = PICOMEDIA_PPM_FORMAT_P3,
            PPMFormatP6         = PICOMEDIA_PPM_FORMAT_P6,
            PPMFormatUnknown    = PICOMEDIA_PPM_FORMAT_UNKNOWN           
        };

        inline std::string PPMFormatToString(PPMFormat format)
        {
            return std::string(PM_ImagePPMFormatToString(format));
        }

        inline PPMFormat PPMDetect(Stream& stream)
        {
            return (PPMFormat)PM_ImagePPMDetect(stream.GetInternalHandlePtr());
        }

        inline PPMFormat PPMDetectFromMemory(PM_Byte* data, PM_Size dataSize)
        {
            return (PPMFormat)PM_ImagePPMDetectFromMemory(data, dataSize);
        }

        inline PPMFormat PPMDetectFromFile(const std::string path)
        {
            return (PPMFormat)PM_ImagePPMDetectFromFile(path.c_str());
        }

        inline PM_Bool PPMReadP6(Stream& stream, Image& image)
        {
            return PM_ImagePPMReadP6(stream.GetInternalHandlePtr(), image.GetImagePtr());
        }

        inline PM_Bool PPMReadP3(Stream& stream, Image& image)
        {
            return PM_ImagePPMReadP3(stream.GetInternalHandlePtr(), image.GetImagePtr());
        }

        inline PM_Bool PPMRead(Stream& stream, Image& image)
        {
            return PM_ImagePPMRead(stream.GetInternalHandlePtr(), image.GetImagePtr());
        }

        inline PM_Bool PPMReadFromMemory(PM_Byte* data, PM_Size dataSize, Image& image)
        {
            return PM_ImagePPMReadFromMemory(data, dataSize, image.GetImagePtr());
        }

        inline PM_Bool PPMReadFromFile(const std::string path, Image& image)
        {
            return PM_ImagePPMReadFromFile(path.c_str(), image.GetImagePtr());
        }

        inline PM_Bool PPMWriteP6(Stream& stream, const Image& image)
        {
            return PM_ImagePPMWriteP6(stream.GetInternalHandlePtr(), image.GetImagePtr());
        }

        inline PM_Bool PPMWriteP3(Stream& stream, const Image& image)
        {
            return PM_ImagePPMWriteP3(stream.GetInternalHandlePtr(), image.GetImagePtr());
        }

        inline PM_Bool PPMWrite(PPMFormat format, Stream& stream, const Image& image)
        {
            return PM_ImagePPMWrite(format, stream.GetInternalHandlePtr(), image.GetImagePtr());
        }

        inline PM_Bool PPMWriteToMemory(PPMFormat format, PM_Byte* data, PM_Size dataSize, const Image& image)
        {
            return PM_ImagePPMWriteToMemory(format, image.GetImagePtr(), data, NULL, dataSize);
        }

        inline PM_Bool PPMWriteToFile(PPMFormat format, const std::string path, const Image& image)
        {
            return PM_ImagePPMWriteToFile(format, path.c_str(), image.GetImagePtr());
        }

    }
}