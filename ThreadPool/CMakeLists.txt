cmake_minimum_required(VERSION 3.3)
project(ThreadPool)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp ThreadPool.cpp ThreadPool.h)
add_executable(ThreadPool ${SOURCE_FILES})