cmake_minimum_required(VERSION 3.5)
project(ConcurrentDataStructure)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# debug
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O0 -g -ggdb")

#set(SOURCE_FILES
#        ConcurrentStack/ConcurrentStack.h)

find_package(BOOST COMPONENTS program_options REQUIRED)

add_subdirectory(ConcurrentStack)
add_subdirectory(tests)
add_subdirectory(lib/googlemock)