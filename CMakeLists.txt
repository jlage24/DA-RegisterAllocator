cmake_minimum_required(VERSION 4.2)
project(DA2026_PRJ2)

set(CMAKE_CXX_STANDARD 20)

add_executable(DA2026_PRJ2 src/main.cpp
        include/Graph.h
        src/graph/LiveRange.h)
