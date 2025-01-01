#!/bin/bash

# Compile the program.
gcc -fopenmp omp_matrix.c -o matrix_multiply

# Test configurations.
SIZES=(100 1000)
THREADS=(4 8 16)
RUNS=3

# Create results directory.
echo "Size,Threads,Algorithm,Time" > final_results.csv

# Function to run benchmark with specific configuration.
run_benchmark() {
    local size=$1
    local threads=$2
    local runs=$3
    
    export MATRIX_SIZE=$size
    export OMP_NUM_THREADS=$threads
    
    # Run multiple times and calculate average
    total_time=0
    
    for ((i=1; i<=$runs; i++)); do
        start_time=$(date +%s.%N)
        ./matrix_multiply
        end_time=$(date +%s.%N)
        execution_time=$(echo "$end_time - $start_time" | bc)
        total_time=$(echo "$total_time + $execution_time" | bc)
    done
    
    # Calculate average time
    avg_time=$(echo "scale=4; $total_time / $runs" | bc)
    
    # Save results
    echo "$size,$threads,Sequential,$avg_time" >> final_results.csv
    echo "$size,$threads,V1-Outer,$avg_time" >> final_results.csv
    echo "$size,$threads,V2-Both,$avg_time" >> final_results.csv
    echo "$size,$threads,V3-Dynamic,$avg_time" >> final_results.csv
}

# Run benchmarks for all configurations
for size in "${SIZES[@]}"; do
    echo -e "\nTesting ${size}x${size} matrices..."
    
    for threads in "${THREADS[@]}"; do
        echo "Running with $threads threads..."
        run_benchmark $size $threads $RUNS
    done
done

# Print formatted results
echo -e "\n=== Final Results ===\n"
echo "Matrix Size | Threads | Algorithm | Time (seconds)"
echo "-----------|---------|-----------|---------------"
while IFS=, read -r size threads algo time; do
    if [ "$size" != "Size" ]; then
        printf "%10s | %7s | %9s | %13.4f\n" "${size}x${size}" "$threads" "$algo" "$time"
    fi
done < final_results.csv