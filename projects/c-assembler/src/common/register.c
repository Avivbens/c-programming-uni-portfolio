#include "register.h"

#include <string.h>

/**
 * Check if the given token is a register
 *
 * @param token The token to check
 *
 * @returns 1 if the token is a register, 0 otherwise
 *
 * @note
 * Register name should look like r[0-7]
 */
int is_register(String token) {
    char first = token[0];
    char second = token[1];

    return (strlen(token) == REGISTER_NAME_LENGTH) && first == 'r' &&
           second >= '0' && second <= '7';
}
