#include "../cli/cli-parser.h"
#include "../constants/constants.h"
#include "../data/symbols-data.h"
#include "../utils/string/string.h"

#define FILL_SUCCESS 1
#define FILL_FAIL 0

/**
 * The Instruction Counter starts at 100 and increments by the number of memory
 * cells each instruction occupies, indicating the next available memory cell.
 */

extern int instCounter;

static int label_registration(String file_name);
static int label_fill(String get_file_name_with_extension);
void create_symbols_table(HashTable *symbolsTable, String *file_names);
static int fill_symbols_table(String *file_names);
void process_symbols_table(String *file_names);
HashTable *create_symbols_table_first_iteration(String *file_names);
void create_symbols_table_second_iteration(String *file_names,
                                           HashTable *symbolsTable);