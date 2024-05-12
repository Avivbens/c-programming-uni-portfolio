#include "./array.h"

#include <stdio.h>

/**
 * Print an array of elements
 *
 * @param array - the array to be printed
 * @param format - the format of the print message
 * @param length - the length of the array
 * @param type - the type of the array: 0: int, 1: double, 2: float, 3: char
 * 4: string, 5: long, 6: short
 */
void print_array(void *array, char *format, int length, int type) {
    int i = 0;

    for (i = 0; i < length; i++) {
        switch (type) {
            case 0:
                printf(format, ((int *)(array))[i]);
                break;
            case 1:
                printf(format, ((double *)(array))[i]);
                break;
            case 2:
                printf(format, ((float *)(array))[i]);
                break;
            case 3:
                printf(format, ((char *)(array))[i]);
                break;
            case 4:
                printf(format, ((char **)(array))[i]);
                break;
            case 5:
                printf(format, ((long *)(array))[i]);
                break;
            case 6:
                printf(format, ((short *)(array))[i]);
                break;
            default:
                printf(format, ((int *)(array))[i]);
                break;
        }
    }
}
