#include <stdio.h>
#include <stdlib.h>

#include "./my-add/my_add.h"
#include "./utils/binary/binary.h"
#include "./utils/number/number.h"
#include "./utils/string/string.h"

int main(void) {
    char a[MAX_SIZE_DECIMAL], b[MAX_SIZE_DECIMAL];
    char *aBinary, *bBinary;
    unsigned int aInt, bInt;

    int sumBinary = 0;
    int sumDecimal = 0;

    printf("\nEnter 2 numbers1: ");
    scanf("%s %s", a, b);
    printf("\nNumbers are: %s, %s\n", a, b);

    aBinary = decimalToBinary(a);
    bBinary = decimalToBinary(b);

    printf("\nBinary numbers are: %s, %s\n", aBinary, bBinary);

    aInt = (int)stringToLong(aBinary);
    bInt = (int)stringToLong(bBinary);

    sumBinary = my_add(aInt, bInt);
    printf("\nSum of binaries: %d\n", sumBinary);

    sumDecimal = (int)stringToLong(binaryToDecimal(numberToString(sumBinary)));
    printf("\nSum of binaries - Decimal: %d\n", sumDecimal);
    return 0;
}
