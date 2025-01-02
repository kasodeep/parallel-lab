#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <stdlib.h>

int is_prime(int number) {
    if (number <= 1) return 0;
    if (number == 2) return 1;
    if (number % 2 == 0) return 0;

    for (int i = 3; i * i <= number; i += 2) {
        if (number % i == 0) return 0;
    }
    return 1;
}

int count_primes(int start, int end, int* primes, int* size) {
    int count = 0;

    #pragma omp parallel
    {
        #pragma omp for schedule(dynamic, 10) reduction(+:count)
        for (int i = start; i <= end; i++) {
            if (is_prime(i)) {
                count++;

                #pragma omp critical 
                {
                    primes[*size] = i;
                    (*size)++;
                }
            }
        }
    }
    return count;
}

int main() {
    int valueN;

    printf("Enter the value of N: ");
    scanf("%d", &valueN);

    int* primes = (int*)malloc((valueN + 1) * sizeof(int));
    int size = 0;

    double start_time = omp_get_wtime();
    int count = count_primes(2, valueN, primes, &size);
    double end_time = omp_get_wtime();

    printf("Number of prime numbers between 2 and %d: %d\n", valueN, count);
    printf("Time taken: %.6f seconds\n", end_time - start_time);

    printf("First 10 primes found (or less if fewer exist):\n");
    for (int i = 0; i < (size < 10 ? size : 10); i++) {
        printf("%d ", primes[i]);
    }
    printf("\n");

    free(primes);
    return 0;
}
