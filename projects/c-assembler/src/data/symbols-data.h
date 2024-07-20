#include "../cli/cli-parser.h"
#include "../constants/constants.h"
#include "../utils/label/label.h"
#include "../utils/string/string.h"
#include "../utils/table/table.h"

typedef struct Symbol {
    char *name;
    char *value;
    int exists;
    int existsExtern;
} Symbol;

int has_label(String name);
Symbol get_label(String name);
int add_label(String name, Symbol value);
HashTable *get_symbols_table(void);
