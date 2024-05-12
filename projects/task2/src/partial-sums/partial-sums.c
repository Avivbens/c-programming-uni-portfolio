#include <stdio.h>
#include <stdlib.h>

/**
 * Compute the partial sums of an array by sum all elements from the beginning
 * @param a - the array of elements
 * @param n - the number of elements
 */
double *partial_sums(double *a, int n) {
    int index = 0;
    double currSum = 0;
    double *sums = (double *)malloc(n * sizeof(double));

    if (sums == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    for (; index < n; index++) {
        currSum += a[index];
        sums[index] = currSum;
    }

    return sums;
}
