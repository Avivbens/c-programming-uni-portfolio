#include "../cli/cli-parser.h"
#include "../constants/constants.h"
#include "../utils/list/list.h"
#include "../utils/string/string.h"

typedef struct Label {
    char *name;
    char *value;
    int exists;
    int existsExtern;
} Label;

int add_label(String name, Label *value);
Label *get_label(String name);
int has_label(String name);
LinkedList *get_labels_list(void);
