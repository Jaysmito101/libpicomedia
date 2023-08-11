#include "libpicomedia/common/utils.h"

static const PM_Char PM_WHITESPACE_CHARACTERS[] = {' ', '\t', '\n', '\r', '\f', '\v', '\0'};
static const PM_Size PM_WHITESPACE_CHARACTERS_COUNT = 7;
static const PM_Size PM_MAX_INTEGER_BUFFER_SIZE = 64;

// -----------------------------------------------------------------------------------------------

PM_Bool PM_CharIsWhiteSpace(PM_Char ch)
{
    for (PM_Size i = 0; i < PM_WHITESPACE_CHARACTERS_COUNT; i++)
    {
        if (ch == PM_WHITESPACE_CHARACTERS[i])
        {
            return PM_TRUE;
        }
    }
    return PM_FALSE;
}

// -----------------------------------------------------------------------------------------------

PM_Int64 PM_ReadASCIIIntegerFromStream(PM_Stream* streamObject)
{
    // Prepare the buffer and index
    static PM_Char integerBuffer[64] = {0};
    PM_Memset(integerBuffer, 0, sizeof(integerBuffer));
    PM_Size integerBufferIndex = 0;
    PM_Char ch = 0;

    while ( ! PM_CharIsWhiteSpace((ch = PM_StreamReadInt8(streamObject))) && (integerBufferIndex < (PM_MAX_INTEGER_BUFFER_SIZE - 1) ) )
    {
        integerBuffer[integerBufferIndex++] = ch;
    }

    return (PM_Int64)atoll(integerBuffer);
}

// -----------------------------------------------------------------------------------------------

PM_Bool PM_IsBigEndian()
{
    union {
        uint32_t i;
        char c[4];
    } e = { 0x01000000 };

    return e.c[0];
}

// -----------------------------------------------------------------------------------------------