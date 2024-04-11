#include <stdio.h>
#include <stdlib.h>

#include "./my-suffix/my_suffix.h"

int main(void) {
    int matchCounter;
    char input;
    char *str = (char *)malloc(MAX_STR_SIZE * sizeof(char));

    printf("\nEnter a single character: \n");
    scanf("%c", &input);

    printf("\nEnter a string to search within: \n");
    scanf("%s", str);

    matchCounter = suffix(str, input);

    free(str);

    if (matchCounter == 0) {
        printf("\nNo matches found!\n");
        return 0;
    }

    printf("\nNumber of matches found: %d\n", matchCounter);
    return 0;
}
