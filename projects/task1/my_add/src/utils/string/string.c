#include "string.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * reverse a string
 * @return reversed string
 */
char *reverse(char *str) {
    int i = 0;
    int length = strlen(str);
    char temp;

    for (i = 0; i < length / 2; i++) {
        temp = str[i];
        str[i] = str[length - i - 1];
        str[length - i - 1] = temp;
    }

    return str;
}

/**
 * convert a string to a long
 */
long stringToLong(const char *str) {
    char *endPtr;
    long result = strtol(str, &endPtr, 10);

    /* Check for conversion errors */
    if (*endPtr != '\0') {
        printf("Error: Invalid input\n");
        exit(EXIT_FAILURE);
    }

    return result;
}
