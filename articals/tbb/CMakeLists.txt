cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

project(cpptest LANGUAGES CXX)

find_package(TBB REQUIRED)
find_package(Threads REQUIRED)

foreach(target IN ITEMS basic parallel_invoke parallel_for)
    add_executable(${target} ${target}.cpp)
    target_link_libraries(${target} PUBLIC TBB::tbb)
    target_link_libraries(${target} PUBLIC Threads::Threads)
endforeach()
