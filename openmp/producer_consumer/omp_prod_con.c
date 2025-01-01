#include<stdio.h>
#include<omp.h>

int main(){
    int i = 0;
    int x = 0;

#pragma omp parallel shared(i, x)
    {
        int tid = omp_get_thread_num();
        if(tid == 0){
            printf("Master thread (producer) is adding data to the buffer for consumers.\n");

            i += 10;
            x = 1;
        }else {
            while (x == 0) {
                printf("Waiting for buffer to be filled. Thread ID: %d\n", omp_get_thread_num());
            }

            #pragma omp critical
            {
                if (i > 0){
                    printf("Data is consumed by Consumer with Thread ID: %d\n", omp_get_thread_num());
                    i -= 5;
                }else {
                    printf("No data found for thread ID: %d\n", omp_get_thread_num());
                }
            }
        }
    }
}