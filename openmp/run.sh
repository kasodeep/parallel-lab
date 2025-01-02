#!/bin/bash

# Function to compile the OpenMP program.
compile_program() {
    local source_file=$1
    local output_file="${source_file%.*}"
    
    gcc -fopenmp "$source_file" -o "$output_file"
    if [ $? -ne 0 ]; then
        echo "Compilation failed!"
        exit 1
    fi
    echo "Compilation successful: $output_file"
}

# Function to run the program with different thread counts
benchmark() {
    local program=$1
    local min_threads=$2
    local max_threads=$3
    local runs=$4
    
    for threads in $(seq $min_threads $max_threads); do
        export OMP_NUM_THREADS=$threads
        echo "Testing with $threads threads..."
        
        for run in $(seq 1 $runs); do
            ./"$program"
        done
        
        echo "----------------------------------------"
    done
    
    echo "Benchmarking complete."
}

# Parse command line arguments.
if [ "$#" -lt 4 ]; then
    echo "Usage: $0 <source_file> <min_threads> <max_threads> <runs_per_thread_count>"
    echo "Example: $0 matrix_multiply.c 1 8 3"
    exit 1
fi

SOURCE_FILE=$1
MIN_THREADS=$2
MAX_THREADS=$3
RUNS=$4

# Main execution.
compile_program "$SOURCE_FILE"
benchmark "${SOURCE_FILE%.*}" $MIN_THREADS $MAX_THREADS $RUNS