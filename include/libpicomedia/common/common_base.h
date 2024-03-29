#ifndef PICOMEDIA_COMMON_BASE_H
#define PICOMEDIA_COMMON_BASE_H

// Standard Library Includes
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Data Types
typedef uint8_t  PM_UInt8;
typedef uint16_t PM_UInt16;
typedef uint32_t PM_UInt32;
typedef uint64_t PM_UInt64;
typedef int8_t   PM_Int8;
typedef int16_t  PM_Int16;
typedef int32_t  PM_Int32;
typedef int64_t  PM_Int64;
typedef float    PM_Float32;
typedef double   PM_Float64;
typedef char     PM_Byte;
typedef char     PM_Char;
typedef size_t   PM_Size;
typedef bool     PM_Bool;

// Macros
#define PICOMEDIA_VERSION_MAJOR 0
#define PICOMEDIA_VERSION_MINOR 1
#define PICOMEDIA_VERSION_PATCH 0

// Platform Detection

#if defined(_WIN32) || defined(_WIN64)
    // Windows
    #define PM_PLATFORM_WINDOWS
#elif defined(__APPLE__)
    // macOS
    #define PM_PLATFORM_MACOS
#elif defined(__linux__)
    // Linux
    #define PM_PLATFORM_LINUX
#else
    #error "Unsupported platform"
#endif

// Compiler Detection
#if defined(_MSC_VER)
    // Microsoft Visual C++
    #define PM_COMPILER_MSVC
#elif defined(__clang__)
    // Clang
    #define PM_COMPILER_CLANG
#elif defined(__GNUC__)
    // GCC
    #define PM_COMPILER_GCC
#else
    #define PM_COMPILER_UNKNOWN
#endif

#ifdef PM_COMPILER_MSVC
    #pragma warning(disable: 4201) // nonstandard extension used: nameless struct/union
#elif defined(PM_COMPILER_CLANG) || defined(PM_COMPILER_GCC)
    #pragma GCC diagnostic ignored "-Wunused-function"
#endif


#define PM_Malloc(size) malloc(size)
#define PM_Free(ptr) free(ptr)
#define PM_Realloc(ptr, size) realloc(ptr, size)
#define PM_New(type) (type*)PM_Malloc(sizeof(type))
#define PM_NewN(type, size) (type*)PM_Malloc(sizeof(type) * size)
#define PM_Delete(ptr) PM_Free(ptr)
#define PM_Memcpy(dest, src, size) memcpy(dest, src, size)
#define PM_Memset(ptr, value, size) memset(ptr, value, size)
#define PM_Malloc(size) malloc(size)
#define PM_Memcmp(ptr1, ptr2, size) memcmp(ptr1, ptr2, size)

#define PM_Max(a, b) ((a) > (b) ? (a) : (b))
#define PM_Min(a, b)  ((a) < (b) ? (a) : (b))

#define PM_TRUE true
#define PM_FALSE false

#if defined(PM_PLATFORM_WINDOWS)
    #define PM_DebugBreak() __debugbreak()
#elif defined(PM_PLATFORM_MACOS) || defined(PM_PLATFORM_LINUX)
    #define PM_DebugBreak() __builtin_trap()
#else
    #error "Unsupported platform"
#endif

// temporary
#define PM_Log(level, ...)  printf(level ":\t"), printf(__VA_ARGS__), printf("\n")
#define PM_LogWarning(...)  PM_Log("Warning", __VA_ARGS__)
#define PM_LogError(...)    PM_Log("Error", __VA_ARGS__), PM_DebugBreak()
#define PM_LogInfo(...)     PM_Log("Info", __VA_ARGS__)

#define PM_Assert(condition) \
    do { \
        if (!(condition)) { \
            PM_LogError("Assertion failed: %s, file %s, line %d", #condition, __FILE__, __LINE__); \
        } \
    } while (0)

 // Platform based API declaration
#ifdef PICOMEDIA_SHARED
    #ifdef _WIN32
        #ifdef PICOMEDIA_BUILD
            #define PICOMEDIA_API __declspec(dllexport)
        #else
            #define PICOMEDIA_API __declspec(dllimport)
        #endif // PICOMEDIA_BUILD
    #else
        #ifdef PICOMEDIA_BUILD
            #define PICOMEDIA_API __attribute__((visibility("default")))
        #else
            #define PICOMEDIA_API
        #endif // PICOMEDIA_BUILD
    #endif // _WIN32
#else
    #define PICOMEDIA_API
#endif // PICOMEDIA_SHARED

#endif // PICOMEDIA_COMMON_BASE_H