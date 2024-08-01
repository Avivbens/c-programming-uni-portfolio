#include "../../cli/cli-parser.h"
#include "../../common/command.h"
#include "../../common/register.h"
#include "../../constants/constants.h"
#include "../../data/macros-data.h"
#include "../../data/symbols-data.h"
#include "../../utils/string/string.h"

/**
 * The Instruction Counter starts at 100 and increments by the number of memory
 * cells each instruction occupies, indicating the next available memory cell.
 */
int instCounter;

void *handle_labels(String *file_names);
