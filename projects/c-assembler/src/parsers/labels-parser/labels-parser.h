#include "../../cli/cli-parser.h"
#include "../../common/command.h"
#include "../../common/register.h"
#include "../../constants/constants.h"
#include "../../data/macros-data.h"
#include "../../data/symbols-data.h"
#include "../../utils/string/string.h"

#define FILL_SUCCESS 1
#define FILL_FAIL 0
#define LABEL_EXTERN_PREFIX ".extern"
#define LABEL_DATA_PREFIX ".data"
#define LABEL_STRING_PREFIX ".string"
#define LABEL_ENTRY_PREFIX ".entry"

typedef enum {
    NOT_LABEL,
    NOT_LABEL_TYPE,
    LABEL_EXTERN,
    LABEL_ENTRY,
    LABEL_DATA,
    LABEL_STRING
} LabelType;

/**
 * The Instruction Counter starts at 100 and increments by the number of memory
 * cells each instruction occupies, indicating the next available memory cell.
 */
int instCounter;

void *handle_labels(String *file_names);
