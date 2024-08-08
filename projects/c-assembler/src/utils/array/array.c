#include "./array.h"

#include <stdio.h>

/**
 * Get the length of a null-terminated array
 *
 * @param array - the array whose length is to be determined
 * @param memory_size_per_cell - the size of each element in the array
 *
 * @return length of the array
 */
int get_array_length(void *array, int memory_size_per_cell) {
    int length = 0;
    char *ptr = (char *)array;

    while (*(ptr + length * memory_size_per_cell) != '\0') {
        length++;
    }

    return length;
}