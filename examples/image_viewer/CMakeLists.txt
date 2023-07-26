set(SOURCES_IMAGE_VIEWER
    ./source/main.c
    )

set(LIBRAIES_IMAGE_VIEWER
    picomedia
    )

set(INCLUDES_IMAGE_VIEWER
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
    ./include
    )

# if liunx add option for x11 and wayland
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    option(USE_X11 "Use X11" ON)
    option(USE_WAYLAND "Use Wayland" OFF)
    
    if (USE_X11)
        add_definitions(-DUSE_X11)
        find_package(X11 REQUIRED)
        list(APPEND INCLUDES_IMAGE_VIEWER ${X11_INCLUDE_DIR})
        list(APPEND LIBRAIES_IMAGE_VIEWER ${X11_LIBRARIES})
        list(APPEND SOURCES_IMAGE_VIEWER ./source/window_manager_x11.c)
        message(STATUS "X11_INCLUDE_DIR: ${X11_INCLUDE_DIR}")
    endif()
    
    if (USE_WAYLAND)
        add_definitions(-DUSE_WAYLAND)
        find_package(Wayland REQUIRED)
        list(APPEND INCLUDES_IMAGE_VIEWER ${WAYLAND_CLIENT_INCLUDE_DIR})
        list(APPEND LIBRAIES_IMAGE_VIEWER ${WAYLAND_CLIENT_LIBRARIES})
        list(APPEND SOURCES_IMAGE_VIEWER ./source/window_manager_wayland.c)
        message(STATUS "WAYLAND_CLIENT_INCLUDE_DIR: ${WAYLAND_CLIENT_INCLUDE_DIR}")
    endif()

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    list(APPEND SOURCES_IMAGE_VIEWER ./source/window_manager_win32.c)
else()
    message(FATAL_ERROR "Unsupported platform for image_viewer example")
endif()

add_executable(image_viewer ${SOURCES_IMAGE_VIEWER})
    
target_link_libraries(image_viewer ${LIBRAIES_IMAGE_VIEWER})
target_include_directories(image_viewer PRIVATE ${INCLUDES_IMAGE_VIEWER})