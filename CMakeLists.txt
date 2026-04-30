cmake_minimum_required(VERSION 4.2)
project(DA2026_PRJ2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)
include_directories(src)

# Collect all source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(regalloc ${SOURCES})

# Enable warnings
target_compile_options(regalloc PRIVATE -Wall -Wextra)