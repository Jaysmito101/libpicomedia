cmake_minimum_required(VERSION 3.13)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(picomedia LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REUIRED True)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(BUILD_STATIC_LIBS "Build static libraries" ON)
option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)

include_directories(./include)
file(GLOB_RECURSE SOURCES "./source/*.c")

# this is the "object library" target: compiles the sources only once
add_library(picomedia OBJECT ${SOURCES})

# shared libraries need PIC
set_property(TARGET picomedia PROPERTY POSITION_INDEPENDENT_CODE 1)

if (BUILD_SHARED_LIBS)
    add_library(picomedia_shared SHARED $<TARGET_OBJECTS:picomedia>)
endif()

if (BUILD_STATIC_LIBS)
    add_library(picomedia_static STATIC $<TARGET_OBJECTS:picomedia>)
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