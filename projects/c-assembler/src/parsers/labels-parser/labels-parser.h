#include "../../cli/cli-parser.h"
#include "../../constants/constants.h"
#include "../../data/symbols-data.h"
#include "../../utils/string/string.h"

#define FILL_SUCCESS 1
#define FILL_FAIL 0
#define LABEL_EXTERN_PREFIX ".extern"

/**
 * The Instruction Counter starts at 100 and increments by the number of memory
 * cells each instruction occupies, indicating the next available memory cell.
 */
int instCounter;

static int is_label(String str);
static int label_registration(String file_name);
static int label_fill(String file_name);
static int is_label_extern(String str);
int is_register(char *token);
int is_command(char *token);