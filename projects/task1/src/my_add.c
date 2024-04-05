#include <stdio.h>

/********************
Author: Yossef Dadon.
ID: 301588273
**********************/

/* Function prototypes */
unsigned int binary_add(unsigned int a, unsigned int b);
void print_binary(unsigned int n);

int main(void)
{
    unsigned int a, b;

    /* Greet the user and explain the program's purpose */
    printf("Welcome to the Binary Adder!\n");
    printf("This program calculates the sum of two numbers and displays the result in binary format.\n\n");

    while (1)
    { /* Start an infinite loop to continuously read pairs of numbers */
        /* Prompt the user for two non-negative numbers */
        printf("Please enter the first number: ");
        if (scanf("%u", &a) != 1)
            break; /* If reading fails (e.g., due to EOF), exit the loop */

        printf("Please enter the second number: ");
        if (scanf("%u", &b) != 1)
            break;

        /* Display the binary sum of the two numbers */
        printf("The sum of %u and %u in binary is: ", a, b);
        print_binary(binary_add(a, b));
        printf("\n");
    }

    return 0;
}
/* Performs binary addition of two unsigned integers */
unsigned int binary_add(unsigned int a, unsigned int b) {
    unsigned int num;
    while (b != 0) {
        num = a & b;  /* Calculate carry bits */
        a = a ^ b;    /* Perform addition without carry */
        b = num << 1; /* Propagate carry */
    }
    return a;
}

/* Prints the binary representation of an unsigned integer */
void print_binary(unsigned int n) {
    const unsigned BIT_LENGTH = 15;
    int i;
    /* Iterate over each bit, starting with the most significant bit */
    for (i = BIT_LENGTH; i >= 0; i--) {
        unsigned int bit = (n >> i) & 1; /* Isolate each bit */
        printf("%u", bit); /* Print the bit */
    }
}
