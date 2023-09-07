#ifndef LIBPICOMEDIA_IMAGE_TRANSFORMS_H
#define LIBPICOMEDIA_IMAGE_TRANSFORMS_H

#include "libpicomedia/common/common.h"
#include "libpicomedia/image/image_base.h"

PM_Bool PICOMEDIA_API PM_ImageTransformsChangeChannelFormat(PM_Image* image, PM_UInt32 newChannelFormat);
PM_Bool PICOMEDIA_API PM_ImageTransformsChangeDataType(PM_Image* image, PM_UInt32 newDataType);
PM_Bool PICOMEDIA_API PM_ImageTransformsFlipHorizontal(PM_Image* image);
PM_Bool PICOMEDIA_API PM_ImageTransformsFlipVertical(PM_Image* image);


#endif // LIBPICOMEDIA_IMAGE_TRANSFORMS_H