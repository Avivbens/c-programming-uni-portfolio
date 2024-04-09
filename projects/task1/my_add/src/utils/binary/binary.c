#include "./binary.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../string/string.h"

/**
 * convert a binary number to a decimal
 * @return 0 for success, -1 for error
 */
char *binaryToDecimal(const char *binary) {
    int length = strlen(binary);
    int i;
    int decimalNumber = 0;
    int base = 1;
    char *res = (char *)malloc(MAX_SIZE_BINARY * sizeof(char));

    for (i = length - 1; i >= 0; i--) {
        if (binary[i] == '1') {
            decimalNumber += base;
        }
        /* Multiply base by 2 for each bit processed */
        base *= 2;
    }

    snprintf(res, MAX_SIZE_BINARY, "%d", decimalNumber);

    return res;
}

/**
 * convert a decimal number to a binary
 * @return 0 for success, -1 for error
 */
char *decimalToBinary(const char *decimal) {
    long decimalNum = stringToLong(decimal);
    char *binaryStr = (char *)malloc(32 * sizeof(char));
    int i = 0;

    if (binaryStr == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    /* Convert decimal to binary */
    while (decimalNum > 0) {
        binaryStr[i++] = (decimalNum % 2) + '0';
        decimalNum = decimalNum / 2;
    }
    binaryStr[i] = '\0';

    binaryStr = reverseString(binaryStr);
    return binaryStr;
}
