#include "string.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * Get the length of a string
 * @param str the string to get the length of
 * @return the length of the string
 */
int stringLength(char str[]) {
    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}

/**
 * Get a substring of a string
 * @param str the string to get the substring from
 * @param start the starting index of the substring
 * @param length the length of the substring
 */
char *subString(char str[], int start, int length) {
    int index = 0;
    char *result = (char *)malloc(length + 1);

    while (index < length) {
        result[index] = str[start + index];
        index++;
    }
    result[index] = '\0';

    return result;
}

/**
 * Print a string
 */
void printString(char str[]) {
    int index = 0;
    while (str[index] != '\0') {
        printf("%c", str[index]);
        index++;
    }
    printf("\n");
}
