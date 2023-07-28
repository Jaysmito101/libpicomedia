#include "libpicomedia/image/ppm/ppm_base.h"

void PM_ImagePPMContextInit(PM_PPMContext* context)
{
    context->ppmFormat = PICOMEDIA_PPM_FORMAT_UNKNOWN;
    context->maxColorValue = 255;
}

void PM_ImagePPMContextDestroy(PM_PPMContext* ppmContext)
{
    PM_ImagePPMContextInit(ppmContext);
}