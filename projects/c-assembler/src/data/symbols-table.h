#include "../../src/cli/cli-parser.h"
#include "../constants/constants.h"
#include "../data/labels-data.h"
#include "../utils/label/label.h"
#include "../utils/string/string.c"
#include "../utils/table/table.h"

typedef struct Symbol {
    char *name;
    char *value;
} Symbol;

static int label_registration(String file_name, HashTable *symbolsTable);
static int label_fill(String file_name, HashTable *symbolsTable);
void create_symbols_table(HashTable *symbolsTable, String *file_names);
void fill_symbols_table(HashTable *symbolsTable, String *file_names);
void process_symbols_table(String *file_names);
HashTable *create_symbols_table_first_iteration(String *file_names);
void create_symbols_table_second_iteration(String *file_names,
                                           HashTable *symbolsTable);
