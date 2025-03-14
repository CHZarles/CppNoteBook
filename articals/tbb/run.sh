#!/bin/bash
set -e

cmake -B build
cmake --build build


# List of executables
executables=( "parallel_invoke" "parallel_for")
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
