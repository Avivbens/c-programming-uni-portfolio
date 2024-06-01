#include "./set.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Create a new set of numbers
 * Each number is represented by a bit in a byte
 *
 * In order to represent numbers 0-127, we need 16 bytes
 * @returns a pointer to the set
 *
 * @note
 * Looking for the number `55`
 * 55 % 8 = 7 // The bit position in the byte
 * 55 / 8 = 6 // The byte position in the set
 *
 * The number `55` is in the 6th byte at the 7th bit
 */
Set *create_set(void) {
    Set *set = (Set *)malloc(MAX_NUM_BYTES * sizeof(Set));

    return set;
}

/**
 * Add a number to a givin set
 * @param - set to add the number to
 * @param - number to add to the set, 0-127
 */
void add_to_set(Set *set, int num) {
    int targetBit = num % BYTE_SIZE;
    int targetByte = num / BYTE_SIZE;

    int hasNum = has(set, num);
    if (hasNum) {
        return;
    }

    set[targetByte] |= (1 << targetBit);
}

/**
 * Remove a number from a givin set
 * @param - set to remove the number from
 * @param - number to remove from the set, 0-127
 */
void remove_from_set(Set *set, int num) {
    int targetBit = num % BYTE_SIZE;
    int targetByte = num / BYTE_SIZE;

    int hasNum = has(set, num);
    if (!hasNum) {
        return;
    }

    set[targetByte] &= ~(1 << targetBit);
}

/**
 * Print the set
 * @param - set to print
 */
void print_set(Set *set) {
    int i, j;

    for (i = 0; i < MAX_NUM_BYTES; i++) {
        for (j = 0; j < BYTE_SIZE; j++) {
            int bit = set[i] & (1 << j);
            if (bit != 0) {
                printf("%d ", i * BYTE_SIZE + j);
            }
        }
    }
    printf("\n");
}

/**
 * Check if a number is in the set
 * @param - set to check
 * @param - number to check in the set, 0-127
 * @returns 1 if the number is in the set, 0 otherwise
 */
int has(Set *set, int num) {
    int targetBit = num % BYTE_SIZE;
    int targetByte = num / BYTE_SIZE;

    int bit = set[targetByte] & (1 << targetBit);
    return bit != 0;
}
