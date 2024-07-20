#include "file.h"

#include <unistd.h>

/**
 * Check if a file exists
 *
 * @param file_path - the file path to check
 * @returns 1 if exists, 0 otherwise
 */
int is_file_exists(String file_path) {
    int hasAccess = access(file_path, F_OK) != -1;
    return hasAccess;
}
