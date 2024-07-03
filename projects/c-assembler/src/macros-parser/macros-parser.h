#include "../constants/constants.h"
#include "../utils/string/string.h"

#define MACRO_TAG_OPEN "macr "
#define MACRO_TAG_CLOSE "endmacr"

#define MAX_MACRO_SIZE sizeof(String) * (MAX_MACROS_LINES * MAX_LINE_LENGTH)

int macro_registration(String file_path);
