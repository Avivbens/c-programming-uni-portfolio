#include <stdint.h>

#define BYTE_SIZE 8
#define MAX_NUM 127
#define MAX_NUM_BYTES (MAX_NUM / BYTE_SIZE + 1)

/**
 * Hold a unique set of numbers, 0-127
 */
typedef uint8_t Set;

Set *create_set(void);
void add_to_set(Set *arr, int num);
void remove_from_set(Set *set, int num);
void print_set(Set *set);
int has(Set *arr, int num);
