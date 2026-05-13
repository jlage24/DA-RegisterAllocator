cmake_minimum_required(VERSION 4.2)
project(DA2026_PRJ2)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)
include_directories(src)

# Main executable
add_executable(regalloc
        src/main.cpp
        src/parser/Parser.cpp
        src/graph/WebBuilder.cpp
        src/graph/InterferenceGraph.cpp
        src/graph/InterferenceGraphBuilder.cpp
        src/allocation/RegisterAllocator.cpp
        src/allocation/SpillAllocator.cpp
        src/allocation/SplitAllocator.cpp
        src/allocation/FreeAllocator.cpp
        src/utils/OutputWriter.cpp
)

# Enable warnings
target_compile_options(regalloc PRIVATE -Wall -Wextra)