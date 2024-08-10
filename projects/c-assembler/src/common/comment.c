#include "comment.h"

#include <stdlib.h>

/**
 * Check if a line is a comment
 *
 * @param line The line to check
 * @return 1 if the line is a comment, 0 otherwise
 */
int is_comment(String line) {
    String trimmed = trim_string(line);
    int is_comment = trimmed[0] == ';';

    return is_comment;
}
