#include "../cli/cli-parser.h"
#include "../constants/constants.h"
#include "../utils/label/label.h"
#include "../utils/list/list.h"
#include "../utils/string/string.h"

typedef struct Symbol {
    char *name;
    char *value;
    int exists;
    int existsExtern;
} Symbol;

int has_label(String name);
Symbol get_label(String name);
int add_label(String name, Symbol value);
LinkedList *get_symbols_list(void);
