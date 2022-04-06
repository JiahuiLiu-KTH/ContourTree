cmake_minimum_required(VERSION 3.21)
project(contourTree)

set(CMAKE_CXX_STANDARD 23)

add_executable(contourTree main.cpp mergeTree.cpp mergeTree.h mesh.cpp mesh.h)
