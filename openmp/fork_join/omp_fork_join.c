#include <stdio.h>
#include <omp.h>

int main() {
    int nthreads, tid;

/* Fork a team of threads giving them their own copies of variables. */
#pragma omp parallel private(nthreads, tid)
    {
        tid = omp_get_thread_num();
        printf("Hello World from thread = %d\n", tid);
        
        /* Only master thread does this. */
        if (tid == 0)
        {
            nthreads = omp_get_num_threads();
            printf("Number of threads = %d Printed by thread with Id: %d\n", nthreads, tid);
        }
    }
    printf("\n");
}