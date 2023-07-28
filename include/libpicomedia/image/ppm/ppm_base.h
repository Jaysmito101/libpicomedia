#ifndef PICOMEDIA_IMAGE_PPM_PPM_BASE_H
#define PICOMEDIA_IMAGE_PPM_PPM_BASE_H

#include "libpicomedia/image/image_base.h"

#define PICOMEDIA_PPM_FORMAT_P3      0x01
#define PICOMEDIA_PPM_FORMAT_P6      0x02
#define PICOMEDIA_PPM_FORMAT_UNKNOWN 0x03

struct PM_PPMContext 
{
    PM_UInt32 ppmFormat;
    PM_UInt64 maxColorValue;
};
typedef struct PM_PPMContext PM_PPMContext;

void PICOMEDIA_API PM_ImagePPMContextInit(PM_PPMContext* ppmContext);
void PICOMEDIA_API PM_ImagePPMContextDestroy(PM_PPMContext* ppmContext);

PM_Bool PICOMEDIA_API PM_ImagePPMDetectFromMemory(const PM_Byte* data, PM_Size dataSize);
PM_Bool PICOMEDIA_API PM_ImagePPMDetectFromFile(const char* filePath);

#endif // PICOMEDIA_IMAGE_PPM_PPM_BASE_H