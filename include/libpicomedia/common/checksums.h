#ifndef PICOMEDIA_COMMON_CHECKSUMS_H
#define PICOMEDIA_COMMON_CHECKSUMS_H

#include "libpicomedia/common/common_base.h"
#include "libpicomedia/common/stream.h"

// Functions for CRC32 checksums
// These algorithms are from https://github.com/stbrumme/crc32

/**
 * @brief Calculates the CRC32 checksum of a given data buffer using the bitwise algorithm.
 * 
 * @param pData The data buffer.
 * @param size The size of the data buffer.
 * @param previousCRC32 The previous CRC32 value. Use 0 for the first call.
 */
PM_UInt32 PICOMEDIA_API PM_CRC32Bitwise(const PM_UInt8* pData, PM_Size size, PM_UInt32 previousCRC32);

/**
 * @brief Calculates the CRC32 checksum of a given data buffer using the half-byte algorithm.
 * 
 * @param pData The data buffer.
 * @param size The size of the data buffer.
 * @param previousCRC32 The previous CRC32 value. Use 0 for the first call.
 */
PM_UInt32 PICOMEDIA_API PM_CRC32HalfByte(const PM_UInt8* pData, PM_Size size, PM_UInt32 previousCRC32);

/**
 * @brief Calculates the CRC32 checksum of a given data buffer using the byte algorithm.
 * 
 * @param pData The data buffer.
 * @param size The size of the data buffer.
 * @param previousCRC32 The previous CRC32 value. Use 0 for the first call.
 */
PM_UInt32 PICOMEDIA_API PM_CRC32OneByte(const PM_UInt8* pData, PM_Size size, PM_UInt32 previousCRC32);


/**
 * @brief Calculates the CRC32 checksum of a given data buffer using the four-byte algorithm.
 * 
 * @param pData The data buffer.
 * @param size The size of the data buffer.
 * @param previousCRC32 The previous CRC32 value. Use 0 for the first call.
 */
PM_UInt32 PICOMEDIA_API PM_CRC32FourByte(const PM_UInt8* pData, PM_Size size, PM_UInt32 previousCRC32);

/**
 * @brief This is just a wrapper for the PM_CRC32FourByte function.
 * 
 * @param pData The data buffer.
 * @param size The size of the data buffer.
 * @param previousCRC32 The previous CRC32 value. Use 0 for the first call.
 */
PM_UInt32 PICOMEDIA_API PM_CRC32(const PM_UInt8* pData, PM_Size size, PM_UInt32 previousCRC32);




#endif // PICOMEDIA_COMMON_CHECKSUMS_H