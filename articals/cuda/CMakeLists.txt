cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CUDA_ARCHITECTURES 89)

project(hello_cuda LANGUAGES CXX CUDA)

add_executable(hello_cuda hello_cuda.cu)
target_include_directories(hello_cuda PUBLIC include)

add_executable(mem_demo mem_demo.cu)
target_include_directories(mem_demo PUBLIC include)


add_executable(poly_calc poly_calc.cu)
target_include_directories(poly_calc PUBLIC include)

add_executable(gravity_simulator gravity_simulator.cu)
target_include_directories(gravity_simulator PUBLIC include)