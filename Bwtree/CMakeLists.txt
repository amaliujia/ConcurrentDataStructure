cmake_minimum_required(VERSION 3.5)
project(Bwtree)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    bwtree.h main.cpp bw.h)

add_executable(Bwtree ${SOURCE_FILES})