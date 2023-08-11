#ifndef PICOMEDIA_COMMON_STRING_UTILS_H
#define PICOMEDIA_COMMON_STRING_UTILS_H

#include "libpicomedia/common/common_base.h"
#include "libpicomedia/common/stream.h"


/**
 * @brief Determines if a character is a whitespace character.
 * 
 * @param ch The character to check.
 * @return PM_Bool Returns PM_TRUE if the character is a whitespace character, PM_FALSE otherwise.
 */
PM_Bool PICOMEDIA_API PM_CharIsWhiteSpace(PM_Char ch);

/**
 * Reads an ASCII integer from the given stream.
 *
 * @param stream The stream to read from.
 * @return The integer value read from the stream.
 */
PM_Int64 PICOMEDIA_API PM_ReadASCIIIntegerFromStream(PM_Stream* stream);


/**
 * @brief Determines if the current system is big-endian or little-endian.
 * 
 * @return PM_Bool Returns PM_TRUE if the system is big-endian, PM_FALSE if it is little-endian.
 */
PM_Bool PICOMEDIA_API PM_IsBigEndian();


#endif // PICOMEDIA_COMMON_STRING_UTILS_H