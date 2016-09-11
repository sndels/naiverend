cmake_minimum_required(VERSION 3.4)

project(skunkwork)

# Enable debug symbols by default
if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE Debugif(MSVC)
    if(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic")
    endif())
endif()

set(EXECUTABLE_NAME ${PROJECT_NAME})

set(SOURCES
    src/gl_core_4_1.c
    src/main.cpp
)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # Add all source files to VS-project
    set(HEADERS
        src/gl_core_4_1.h
    )
    add_executable(${EXECUTABLE_NAME} ${SOURCES} ${HEADERS})
    target_link_libraries(${EXECUTABLE_NAME} "opengl32" "glfw3dll")
    # Copy dlls to build directory
    add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/lib/win $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
else()
    add_executable(${EXECUTABLE_NAME} ${SOURCES})
    # Set correct compiler std
    target_compile_features(${EXECUTABLE_NAME} PRIVATE cxx_range_for)
    # Find OpenGL
    find_package(OpenGL REQUIRED)
    if(OPENGL_FOUND)
        include_directories(${OPENGL_INCLUDE_DIR})
        target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_LIBRARIES})
    endif()
    set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")
    # Find GLFW
    find_package(GLFW REQUIRED)
    if(GLFW_FOUND)
        include_directories(${GLFW_INCLUDE_DIR})
        target_link_libraries(${EXECUTABLE_NAME} ${GLFW_LIBRARIES})
    endif()
endif()
