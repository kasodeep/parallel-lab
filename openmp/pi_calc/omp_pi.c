#include <stdio.h>
#include <omp.h>

int main() {
    int num_steps = 10000;
    double start_time, end_time, time_taken;
    
    start_time = omp_get_wtime();
    
    double step = 1.0/(double)num_steps;
    double sum = 0.0;
    
    #pragma omp parallel
    {
        double x;
        #pragma omp for reduction(+:sum) schedule(static)
        for (int i = 0; i < num_steps; i++) {
            x = (i + 0.5) * step;
            sum += 4.0/(1.0 + x*x);
        }
    }
    
    double pi = step * sum;
    
    end_time = omp_get_wtime();
    time_taken = (end_time - start_time) * 1000000;
    
    printf("The Value of PI is %.10lf\n", pi);
    printf("Time taken: %.2f microseconds (%.6f seconds)\n", time_taken, time_taken/1000000);
    
    return 0;
}