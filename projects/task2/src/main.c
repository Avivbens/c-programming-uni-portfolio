#include <stdio.h>
#include <stdlib.h>

#include "./partial-sums/partial-sums.h"
#include "./utils/array/array.h"

int main(void) {
    int elements_count = 0, index = 0;
    double *elements;
    double *result;

    printf("Enter the number of elements: \n");
    scanf("%d", &elements_count);

    elements = (double *)malloc(elements_count * sizeof(double));
    if (elements == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    /**
     * Scan all elements
     */
    printf("Enter all related elements: \n");
    for (; index < elements_count; index++) {
        scanf("%lf", &elements[index]);
    }
    elements[index] = '\0';

    /**
     * Print all elements
     */
    printf("The elements are: \n");
    print_array(elements, (char *)"%.2lf \n", elements_count, 1);

    printf("\n-------------------\n\n");

    /**
     * Results
     */
    printf("The results are: \n");
    result = partial_sums(elements, elements_count);
    print_array(result, (char *)"%.2lf \n", elements_count, 1);

    free(elements);
    free(result);

    return 0;
}
