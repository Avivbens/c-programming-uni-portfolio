#include "../common/command.h"
#include "../constants/constants.h"
#include "../data/extern-data.h"
#include "../data/symbols-data.h"
#include "../parsers/labels-parser/labels-parser.h"
#include "../utils/number/number.h"
#include "../utils/string/string.h"

#ifndef OUTPUT_GENERATOR_H
#define OUTPUT_GENERATOR_H

int handle_output(String *file_paths);

#endif

/**
 * ---------------------
 * The operand address mode should be:
 *
 * 0 - numbers, start with #. We need to cast it to binary. Would be
 * the entire binary, but the last 3 digits (ARE)
 *
 * 1 - labels, show the address of the label (based on the labels
 * system). Would be the entire binary, but the last 3 digits (ARE)
 *
 * 2 - pointer to register, show the register number. Would be the
 * entire binary, but the last 3 digits (ARE)
 *
 * 3 - register, show the number of the register. Would be the
 * entire binary, but the last 3 digits (ARE)
 *
 *
 * These need to be correlated to the operand itself (source /
 * dest).
 *
 *
 * In case the 2 operands are registers/pointers to registers, they
 * would be translated into one line.
 *
 * The source should be from 6-8 bits, the dest should be 3-5 bits.
 * ---------------------
 */
