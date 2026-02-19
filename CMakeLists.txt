cmake_minimum_required(VERSION 3.16)
project(ThreadPool)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare (
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()


add_executable(threadpool_tests
    tests/threadpool_test.cpp
)

target_include_directories(threadpool_tests
    PUBLIC include
)

target_link_libraries(threadpool_tests
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(threadpool_tests)