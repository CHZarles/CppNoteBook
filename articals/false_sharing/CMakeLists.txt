cmake_minimum_required(VERSION 3.16)
project(false_sharing_demo LANGUAGES CXX)

find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(false_sharing_demo src/false_sharing_demo.cpp)
target_include_directories(false_sharing_demo PRIVATE include)
target_link_libraries(false_sharing_demo PRIVATE Threads::Threads)

add_executable(false_sharing_test tests/test_false_sharing.cpp)
target_include_directories(false_sharing_test PRIVATE include)
target_link_libraries(false_sharing_test PRIVATE Threads::Threads)

enable_testing()
add_test(NAME false_sharing_test COMMAND false_sharing_test)
