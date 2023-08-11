cmake_minimum_required(VERSION 3.13)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(picomedia LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REUIRED True)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(BUILD_STATIC_LIBS "Build static libraries" OFF)
option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)

if (BUILD_TESTS)
    message(STATUS "Enabling CXX for tests")
    enable_language(CXX)
    
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REUIRED True)
endif()

include_directories(./include)

set(SOURCES
    # Common
    source/common/common_stream.c
    source/common/common_utils.c
    # Image
    source/image/image_base.c
    # Image -> PPM
    source/image/ppm/ppm_base.c
    source/image/ppm/ppm_detect.c    
    source/image/ppm/ppm_read.c    
    source/image/ppm/ppm_write.c    
    # Image -> BMP
    source/image/bmp/bmp_base.c
    source/image/bmp/bmp_detect.c
    source/image/bmp/bmp_read.c
    source/image/bmp/bmp_write.c
    )


# this is the "object library" target: compiles the sources only once
add_library(picomedia OBJECT ${SOURCES})

# shared libraries need PIC
set_property(TARGET picomedia PROPERTY POSITION_INDEPENDENT_CODE 1)


if (NOT BUILD_SHARED_LIBS AND NOT BUILD_STATIC_LIBS)
    message(FATAL_ERROR "Both shared and static libraries are disabled")
endif()

if (BUILD_SHARED_LIBS AND BUILD_STATIC_LIBS)
    message(FATAL_ERROR "Both shared and static libraries are enabled")
endif()
    
if (BUILD_SHARED_LIBS)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    add_library(picomedia_shared SHARED $<TARGET_OBJECTS:picomedia>)
    target_compile_definitions(picomedia_shared PRIVATE PICOMEDIA_SHARED PICOMEDIA_BUILD)
endif()

if (BUILD_STATIC_LIBS)
    add_library(picomedia_static STATIC $<TARGET_OBJECTS:picomedia>)
    target_compile_definitions(picomedia_static PRIVATE PICOMEDIA_STATIC PICOMEDIA_BUILD)
endif()

if (BUILD_TESTS)
    add_subdirectory(tests)
endif()

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(MSVC)
    # remove unnecessary warnings on not using secure CRT functions
    add_definitions(-D_CRT_SECURE_NO_WARNINGS) 
    # max warning level and warnings as errors
    target_compile_options(picomedia PRIVATE /W4 /WX)
else()
    # max warning level and warnings as errors
    target_compile_options(picomedia PRIVATE -Wall -Wextra -Wpedantic -Werror -Woverlength-strings)
endif()