#include <math.h>
#include <stdio.h>

#include "../utils/number/number.h"
#include "../utils/string/string.h"

unsigned int my_add(unsigned int a, unsigned int b) {
    int i = 0, remainder = 0;
    int currentSum = 0;
    long result = 0;

    while (a != 0 || b != 0) {
        currentSum = ((a % 10) + (b % 10) + remainder);
        remainder = currentSum / 2;

        currentSum = currentSum % 2;
        result += (currentSum * pow(10, i));
        a /= 10;
        b /= 10;

        i++;
    }

    return (int)result;
}
