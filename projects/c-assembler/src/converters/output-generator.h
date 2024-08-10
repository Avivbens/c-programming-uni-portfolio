#include "../common/command.h"
#include "../constants/constants.h"
#include "../data/symbols-data.h"
#include "../parsers/labels-parser/labels-parser.h"
#include "../utils/number/number.h"
#include "../utils/string/string.h"

#ifndef OUTPUT_GENERATOR_H
#define OUTPUT_GENERATOR_H

typedef enum {
    ERROR = -1,
    IMMEDIATE_ADDRESS_MODE = 0,
    DIRECT_ADDRESS_MODE = 1,
    INDIRECT_ACCUMULATED_ADDRESS_MODE = 2,
    DIRECT_ACCUMULATED_ADDRESS_MODE = 3
} AddressMode;

int handle_output(String *file_paths);

#endif
