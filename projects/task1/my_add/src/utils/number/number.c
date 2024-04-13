#include "./number.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../string/string.h"

/**
 * get the length of a number
 * @return length of the number
 */
int numLength(const int number) {
    char strNumber[100];
    int length;

    sprintf(strNumber, "%d", number);
    length = strlen(strNumber);

    return length;
}

/**
 * reverse a number
 * @return reversed number
 */
int reverseNumber(int number) {
    int reversedNumber = 0;

    while (number != 0) {
        reversedNumber = reversedNumber * 10 + number % 10;
        number /= 10;
    }

    return reversedNumber;
}

/**
 * convert a number to a string
 * @return string representation of the number
 */
char *numberToString(int number) {
    char *result = (char *)malloc(100 * sizeof(char));
    int i = 0;

    if (result == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    while (number != 0) {
        result[i++] = (number % 10) + '0';
        number /= 10;
    }

    result[i] = '\0';

    result = reverseString(result);

    return result;
}
