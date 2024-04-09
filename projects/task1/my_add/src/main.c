#include <stdio.h>
#include <stdlib.h>

#include "./my-add/my_add.h"
#include "./utils/binary/binary.h"

int main(void) {
    char a[MAX_SIZE_DECIMAL], b[MAX_SIZE_DECIMAL];
    char *aBinary, *bBinary;

    printf("\nEnter 2 numbers: ");
    scanf("%s %s", a, b);
    printf("\nNumbers are: %s, %s\n", a, b);

    aBinary = decimalToBinary(a);
    bBinary = decimalToBinary(b);

    printf("\nBinary numbers are: %s, %s\n", aBinary, bBinary);

    aBinary = binaryToDecimal(aBinary);
    bBinary = binaryToDecimal(bBinary);

    printf("\nDecimal numbers are: %s, %s\n", aBinary, bBinary);
    return 0;
}
