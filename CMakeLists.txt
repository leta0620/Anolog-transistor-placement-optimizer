cmake_minimum_required(VERSION 3.20)
project(AnalogTransistorPlacementOptimizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)
file(GLOB_RECURSE HEADER_FILES  CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/header/*.h)

add_executable(atpo ${SOURCES} ${HEADER_FILES})
    
target_include_directories(atpo PRIVATE ${CMAKE_SOURCE_DIR}/header)

if (MSVC)
    target_compile_options(atpo PRIVATE /W4 /permissive-)
else()
    target_compile_options(atpo PRIVATE -Wall -Wextra -Wpedantic)
endif()

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${SOURCES})
