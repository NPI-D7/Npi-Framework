cmake_minimum_required(VERSION 3.18)

# Setup Toolchain if not specified
# Could propably avoided by using arm-none-eabi-cmake
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    if(DEFINED ENV{DEVKITPRO})
        set(CMAKE_TOOLCHAIN_FILE "$ENV{DEVKITPRO}/cmake/3DS.cmake" CACHE PATH "toolchain file")
    else()
        message(FATAL_ERROR "Please define DEVKITPRO to point to your SDK path!")
    endif()
endif()

# Set Project
project(npi-framework LANGUAGES C CXX ASM VERSION 2.0.0)

# Enable Compile Command Export
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Force C++ 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Set Special C and CXX flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-psabi -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS} -fno-rtti")

add_subdirectory(../palladium palladium) # Only use stock palladium yet
# add_subdirectory(palladium/backends/3ds)

add_library(${PROJECT_NAME} STATIC
    source/renderer/3d/Material.cpp
    source/renderer/3d/Mesh.cpp
    source/renderer/roc/Cube.cpp
    source/renderer/roc/Model.cpp
    source/renderer/roc/Text.cpp
    source/renderer/Camera.cpp
    source/renderer/Color.cpp
    source/renderer/Polygon.cpp
    source/renderer/RenderContext.cpp
    source/renderer/Renderer.cpp
    source/renderer/RenderTarget.cpp
    source/renderer/Texture.cpp
    source/nmdl.cpp
    source/Npi.cpp
    source/Shader.cpp
    source/stb.cpp

    source/renderer/rn3d_shbin.S
)

# Set dependencies, include dirs and definitions
target_link_libraries(${PROJECT_NAME} PUBLIC m palladium citro2d citro3d ctru)
target_include_directories(${PROJECT_NAME} PUBLIC
    include
    include/ext
    ${DEVKITPRO}/portlibs/3ds/include
    ${CMAKE_BINARY_DIR}/.dkp-generated/rn3d_shbin
)
