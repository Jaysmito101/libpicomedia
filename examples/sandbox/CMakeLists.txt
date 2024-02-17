set(SOURCES_SANDBOX
    ./source/main.c
    )

set(LIBRAIES_SANDBOX
    picomedia
    )

set(INCLUDES_SANDBOX
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
    ./include
    )

add_executable(sandbox ${SOURCES_SANDBOX})
    
target_link_libraries(sandbox ${LIBRAIES_SANDBOX})
target_include_directories(sandbox PRIVATE ${INCLUDES_SANDBOX})