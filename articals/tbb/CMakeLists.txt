cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

project(cpptest LANGUAGES CXX)
set(CMAKE_BUILD_TYPE Release)

find_package(TBB REQUIRED)
find_package(Threads REQUIRED)

foreach(target IN ITEMS tb_run parallel_scan parallel_reduce parallel_invoke parallel_for nested_parallel_for
  assign_task filter_example divide_conquer_sort_example pipeline_example)
    add_executable(${target} ${target}.cpp)
    target_link_libraries(${target} PUBLIC TBB::tbb)
    target_link_libraries(${target} PUBLIC Threads::Threads)
endforeach()


find_package(benchmark REQUIRED)
add_executable(benchmark_test benchmark_test.cpp)
target_link_libraries(benchmark_test PRIVATE benchmark::benchmark TBB::tbb )
