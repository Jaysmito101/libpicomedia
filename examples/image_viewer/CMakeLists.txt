set(SOURCES_IMAGE_VIEWER
    ./source/main.c
    )


# if liunx add option for x11 and wayland
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    option(USE_X11 "Use X11" ON)
    option(USE_WAYLAND "Use Wayland" OFF)
    
    if (USE_X11)
        add_definitions(-DUSE_X11)
        find_package(X11 REQUIRED)
        include_directories(${X11_INCLUDE_DIR})
        target_link_libraries(image_viewer ${X11_LIBRARIES})
        list(APPEND SOURCES_IMAGE_VIEWER ./source/window_manager_x11.c)
    endif()
    
    if (USE_WAYLAND)
        add_definitions(-DUSE_WAYLAND)
        find_package(Wayland REQUIRED)
        include_directories(${WAYLAND_CLIENT_INCLUDE_DIR})
        target_link_libraries(image_viewer ${WAYLAND_CLIENT_LIBRARIES})
        list(APPEND SOURCES_IMAGE_VIEWER ./source/window_manager_wayland.c)
    endif()

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    list(APPEND SOURCES_IMAGE_VIEWER ./source/window_manager_win32.c)
else()
    message(FATAL_ERROR "Unsupported platform for image_viewer example")
endif()
    
add_executable(image_viewer ${SOURCES_IMAGE_VIEWER})
target_link_libraries(image_viewer picomedia)
target_include_directories(image_viewer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../include ./include)