#include "./file-handler.h"

#include <unistd.h>

int is_file_exists(String file_path) {
    int hasAccess = access(file_path, F_OK) != -1;
    return hasAccess;
}
