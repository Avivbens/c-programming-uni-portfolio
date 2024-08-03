#include "../../cli/cli-parser.h"
#include "../../common/command.h"
#include "../../common/register.h"
#include "../../constants/constants.h"
#include "../../data/macros-data.h"
#include "../../data/symbols-data.h"
#include "../../utils/string/string.h"

#ifndef LABELS_PARSER_H
#define LABELS_PARSER_H

/**
 * The Instruction Counter begins at 100 and increases by the number of memory
 * cells each instruction uses.
 *
 * This indicates the next available memory cell.
 */
/* int instruction_counter = SYMBOL_START_POINT; */

/**
 * The Data Counter serves a similar purpose as the Instruction Counter but is
 * specifically for data.
 *
 * For safety reasons, they are kept separate.
 *
 * TODO - keep them separate
 */
/* int data_counter = SYMBOL_START_POINT + 20; */

void *handle_labels(String *file_names);

#endif
