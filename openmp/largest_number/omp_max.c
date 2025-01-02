#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main() {
    int numberOfElements, iIterator;
    double start_time, end_time, time_taken;
    
    printf("Enter the Number of Elements: ");
    scanf("%d", &numberOfElements);
    int arrayInput[numberOfElements];
    
    int randRange = rand();

    printf("Generating random elements:\n");
    for(iIterator = 0; iIterator < numberOfElements; iIterator++) {
        arrayInput[iIterator] = rand() % randRange;
    }
    printf("\n");
    
    start_time = omp_get_wtime();
    
    int currentMax = arrayInput[0];
    #pragma omp parallel for reduction(max:currentMax)
    for(iIterator = 1; iIterator < numberOfElements; iIterator++) {
        if(arrayInput[iIterator] > currentMax) {
            currentMax = arrayInput[iIterator];
        }
    }
    
    end_time = omp_get_wtime();
    time_taken = (end_time - start_time) * 1000000;
    
    printf("The Maximum Element is: %d\n", currentMax);
    printf("Time taken: %.2f microseconds (%.6f seconds)\n", time_taken, time_taken / 1000000);
    
    return 0;
}
