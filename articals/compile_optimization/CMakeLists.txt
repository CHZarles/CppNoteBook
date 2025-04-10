cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

project(cpptest LANGUAGES CXX)
set(CMAKE_BUILD_TYPE Release)

find_package(Threads REQUIRED)
find_package(benchmark REQUIRED)


foreach(target IN ITEMS test_loop)
    add_executable(${target} ${target}.cpp)
    target_link_libraries(${target} PUBLIC Threads::Threads benchmark::benchmark)
endforeach()


