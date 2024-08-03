#include "../cli/cli-parser.h"
#include "../constants/constants.h"
#include "../utils/list/list.h"
#include "../utils/string/string.h"

typedef enum {
    LABEL_VIOLATION = -1,
    NOT_LABEL,
    NOT_LABEL_TYPE,
    LABEL_EXTERN,
    LABEL_ENTRY,
    LABEL_DATA,
    LABEL_STRING
} LabelType;

typedef struct Label {
    String name;
    int memory_address;
    int is_defined;
    int has_extern;
    int has_entry;
} Label;

#define FILL_SUCCESS 1
#define FILL_FAIL 0
#define LABEL_EXTERN_PREFIX ".extern"
#define LABEL_DATA_PREFIX ".data"
#define LABEL_STRING_PREFIX ".string"
#define LABEL_ENTRY_PREFIX ".entry"

int add_label(String name, LabelType type, int memory_address);
Label *get_label(String name);
int has_label(String name);
LinkedList *get_labels_list(void);
