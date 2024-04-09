#include "./number.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../string/string.h"

int numLength(const int number) {
    char strNumber[100];
    int length;

    sprintf(strNumber, "%d", number);
    length = strlen(strNumber);

    return length;
}

int reverseNumber(int number) {
    int reversedNumber = 0;

    while (number != 0) {
        reversedNumber = reversedNumber * 10 + number % 10;
        number /= 10;
    }

    return reversedNumber;
}

char *numberToString(int number) {
    char *result = (char *)malloc(100 * sizeof(char));
    int i = 0;

    while (number != 0) {
        result[i++] = (number % 10) + '0';
        number /= 10;
    }

    result[i] = '\0';

    result = reverseString(result);

    return result;
}

char *arrayToString(int *array) {
    char *result = (char *)malloc(100 * sizeof(char));
    int length = arraySize(array);
    int i = 0;

    for (i = 0; i < length; i++) {
        result[i] = array[i] + '0';
    }

    result[i] = '\0';

    return result;
}

int arraySize(int *array) {
    int length = 0;

    while (array[length] != EOF) {
        length++;
    }

    return length;
}
