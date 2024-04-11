#include <stdio.h>
#include <stdlib.h>

#include "../utils/string/string.h"

/**
 * Find and print all suffixes of a string that start with a given character
 * @param str the string to search within
 * @param c the character to search for
 */
int suffix(char str[], char c) {
    int index = 0;
    int matchCounter = 0;

    int strLength = stringLength(str);
    char *result = (char *)malloc((strLength + 2) * sizeof(char));
    char *temp = (char *)malloc(strLength * sizeof(char));

    printf("\nMatches Analyze: \n\n");

    while (index < strLength) {
        if (str[index] == c) {
            temp = subString(str, index, strLength - index);
            sprintf(result, "* %s", temp);
            printString(result);
            matchCounter++;
        }
        index++;
    }

    free(result);
    free(temp);

    return matchCounter;
}
