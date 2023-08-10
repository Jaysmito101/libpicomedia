#pragma once

#include "libpicomedia/common/common.hpp"
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

    }
}