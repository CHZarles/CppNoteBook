#!/bin/bash
set -e

cmake -B build
cmake --build build

# ./build/pipeline_example
# exit

# List of executables
executables=( "test_loop")

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

