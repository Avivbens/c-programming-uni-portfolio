#include "../../cli/cli-parser.h"
#include "../../constants/constants.h"
#include "../../data/macros-data.h"
#include "../../utils/string/string.h"

#define MACRO_TAG_OPEN "macr "
#define MACRO_TAG_CLOSE "endmacr"

#define MAX_MACRO_SIZE sizeof(String) * (MAX_MACROS_LINES * MAX_LINE_LENGTH)

String *handle_macros(String *files);
