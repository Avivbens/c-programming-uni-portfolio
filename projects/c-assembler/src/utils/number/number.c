#include "number.h"

#include <ctype.h>

/**
 * Check if a string is a number
 *
 * @param str the string to check
 *
 * @returns 1 if the string is a number, 0 otherwise
 *
 * @note
 * This function does check for negative numbers
 */
int is_number(String str) {
    int i;
    if (str[0] == '-' || str[0] == '+') {
        str++;
    }

    for (i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}
