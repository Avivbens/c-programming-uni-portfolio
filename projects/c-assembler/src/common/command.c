#include "command.h"

#include <string.h>

/**
 * Check if the given token is a valid command
 *
 * @param token The token to check
 *
 * @returns 1 if the token is a command, 0 otherwise
 */
int is_command(String token) {
    int length = strlen(token);
    if (length > MAX_COMMAND_LENGTH || length < MIN_COMMAND_LENGTH) {
        return 0;
    }

    return 1;
}
