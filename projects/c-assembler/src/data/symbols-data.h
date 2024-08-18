#include <stdio.h>

#include "../cli/cli-parser.h"
#include "../constants/constants.h"
#include "../utils/list/list.h"
#include "../utils/string/string.h"

#ifndef SYMBOLS_DATA_H
#define SYMBOLS_DATA_H

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
    int memory_address;
    int is_defined;
    int has_extern;
    int has_entry;
    String name;
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

int label_list_size(void);
void iterate_labels(void (*callback)(Label *, String, FILE *), String context,
                    FILE *exec);

int get_instruction_counter(int increment);
int get_data_counter(int increment);

void debug_labels(void);

#endif
