include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

add_executable(stackTest
        ConcurrentStackTest.cpp)

target_link_libraries(stackTest gtest gtest_main)
target_link_libraries (stackTest glog)

if (APPLE)
    add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
    add_definitions(-D__GLIBCXX__)
endif (APPLE)