#!/bin/bash
set -e

cmake -B build
cmake --build build

# ./build/pipeline_example
# exit

# List of executables
executables=( "parallel_invoke" "parallel_for" "parallel_reduce" "parallel_scan" "nested_parallel_for" "assign_task" "filter_example" "divide_conquer_sort_example" "pipeline_example")

#"basic"
# Run each executable
for exe in "${executables[@]}"; do
    exe_path="./build/${exe}"
    if [ -f "$exe_path" ]; then
        echo "Running $exe..."
        $exe_path
    else
        echo "Executable $exe not found."
    fi
done

./build/benchmark_test
