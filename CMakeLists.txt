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

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(CLANG_COVERAGE_LINK_FLAGS  "-framework Cocoa -framework IOKit -framework CoreVideo")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CLANG_COVERAGE_LINK_FLAGS}")
endif()

set(EXECUTABLE_NAME ${PROJECT_NAME})

set(SOURCES
    src/basic_program.cpp
    src/camera.cpp
    src/gl_core_4_1.c
    src/input_handler.cpp
    src/main.cpp
    src/mesh.cpp
    src/mesh_parser.cpp
    src/scene.cpp
    src/shader_program.cpp
)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # Add all source files to VS-project
    set(HEADERS
        src/basic_program.hpp
        src/camera.hpp
        src/gl_core_4_1.h
        src/input_handler.hpp
        src/math_types.hpp
        src/mc_tables.hpp
        src/mesh_parser.hpp
        src/scene.hpp
        src/shader_program.hpp
    )
    # Override debugger working directory
    write_file("${PROJECT_NAME}.vcxproj.user"
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>     \
        <Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">
        <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">
            <LocalDebuggerWorkingDirectory>$(OutDir)</LocalDebuggerWorkingDirectory>
            <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
        </PropertyGroup>
        <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">
            <LocalDebuggerWorkingDirectory>$(OutDir)</LocalDebuggerWorkingDirectory>
            <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
        </PropertyGroup>
        </Project>")
    #Config build
    add_executable(${EXECUTABLE_NAME} ${SOURCES} ${HEADERS})
    target_link_libraries(${EXECUTABLE_NAME} "opengl32" "glfw3dll")
    # Copy dlls to build directory after build
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

# Copy shaders to build directory
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/src/shader $<TARGET_FILE_DIR:${PROJECT_NAME}>/shader)
# Copy resources to build directory
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)
