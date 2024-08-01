#include "../cli/cli-parser.h"
#include "../constants/constants.h"
#include "../utils/list/list.h"
#include "../utils/string/string.h"

typedef struct Label {
    char *name;
    char *value;
    LabelType type; /* entry/extern/data/string */
} Label;

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

int add_label(String name, Label *value, int type);
Label *get_label(String name);
int has_label(String name);
LinkedList *get_labels_list(void);
