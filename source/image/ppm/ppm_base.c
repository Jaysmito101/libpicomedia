#include "libpicomedia/image/ppm/ppm.h"

// -----------------------------------------------------------------------------------------------

const PM_Char* PM_ImagePPMFormatToString(PM_UInt32 ppmFormat)
{
    switch (ppmFormat)
    {
        case PICOMEDIA_PPM_FORMAT_P3: return "P3";
        case PICOMEDIA_PPM_FORMAT_P6: return "P6";
        default:                      return "UNKNOWN";
    }
}

// -----------------------------------------------------------------------------------------------