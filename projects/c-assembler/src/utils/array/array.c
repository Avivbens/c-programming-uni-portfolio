#include "array.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * Get the length of a null-terminated array
 *
 * @param array - the array whose length is to be determined
 * @param memory_size_per_cell - the size of each element in the array
 *
 * @return length of the array
 */
int get_string_array_length(String *array) {
    int length = 0;

    while (array[length] != NULL) {
        length++;
    }

    return length;
}

void free_string_array_recursively(String *array, int length) {
    int i;
    for (i = 0; i < length; i++) {
        if (array[i] == (String)'\0') {
            continue;
        }

        free(array[i]);
        array[i] = NULL;
    }

    free(array);
    array = NULL;
}
