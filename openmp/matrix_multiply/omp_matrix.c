#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

void initialize_matrix(int** matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = rand() % 100;
        }
    }
}

// Sequential version
void matrix_multiply_sequential(int** A, int** B, int** C, int size) {
    double start = omp_get_wtime();
    
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            C[i][j] = 0;
            for (int k = 0; k < size; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    
    double end = omp_get_wtime();
    printf("\n=== Sequential Multiplication ===\n");
    printf("Matrix size: %dx%d\n", size, size);
    printf("Execution time: %.4f seconds\n", end - start);
}

// V1: Only outer loop parallel
void matrix_multiply_v1(int** A, int** B, int** C, int size) {
    double start = omp_get_wtime();
    
    #pragma omp parallel for
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            C[i][j] = 0;
            for (int k = 0; k < size; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    
    double end = omp_get_wtime();
    printf("\n=== V1: Outer Loop Parallel ===\n");
    printf("Matrix size: %dx%d\n", size, size);
    printf("Number of threads: %d\n", omp_get_max_threads());
    printf("Execution time: %.4f seconds\n", end - start);
}

// V2: Both loops parallel
void matrix_multiply_v2(int** A, int** B, int** C, int size) {
    double start = omp_get_wtime();
    
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            C[i][j] = 0;
            for (int k = 0; k < size; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    
    double end = omp_get_wtime();
    printf("\n=== V2: Both Loops Parallel (collapse) ===\n");
    printf("Matrix size: %dx%d\n", size, size);
    printf("Number of threads: %d\n", omp_get_max_threads());
    printf("Execution time: %.4f seconds\n", end - start);
}

// V3: Dynamic scheduling
void matrix_multiply_v3(int** A, int** B, int** C, int size) {
    double start = omp_get_wtime();
    
    #pragma omp parallel for collapse(2) schedule(dynamic)
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            C[i][j] = 0;
            for (int k = 0; k < size; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    
    double end = omp_get_wtime();
    printf("\n=== V3: Dynamic Scheduling ===\n");
    printf("Matrix size: %dx%d\n", size, size);
    printf("Number of threads: %d\n", omp_get_max_threads());
    printf("Execution time: %.4f seconds\n", end - start);
}

int main() {
    int size;
    printf("\n====================================\n");
    printf("Matrix Multiplication Performance Test\n");
    printf("====================================\n");
    
    // Get matrix size from environment variable or default to 100
    char* size_env = getenv("MATRIX_SIZE");
    size = size_env ? atoi(size_env) : 100;
    
    // Allocate matrices
    int** A = (int**)malloc(size * sizeof(int*));
    int** B = (int**)malloc(size * sizeof(int*));
    int** C = (int**)malloc(size * sizeof(int*));
    
    for (int i = 0; i < size; i++) {
        A[i] = (int*)malloc(size * sizeof(int));
        B[i] = (int*)malloc(size * sizeof(int));
        C[i] = (int*)malloc(size * sizeof(int));
    }
    
    // Initialize matrices
    srand(time(NULL));
    initialize_matrix(A, size);
    initialize_matrix(B, size);
    
    // Run all variants
    matrix_multiply_sequential(A, B, C, size);
    matrix_multiply_v1(A, B, C, size);
    matrix_multiply_v2(A, B, C, size);
    matrix_multiply_v3(A, B, C, size);
    
    // Free memory
    for (int i = 0; i < size; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);
    
    return 0;
}