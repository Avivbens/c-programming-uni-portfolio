#include <stdio.h>
#include <stdlib.h>

#include "../constants/constants.h"

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
        fprintf(stderr, "Memory allocation failed\n");
        exit(MEMORY_OVERFLOW);
    }

    for (; index < n; index++) {
        currSum += a[index];
        sums[index] = currSum;
    }

    return sums;
}
