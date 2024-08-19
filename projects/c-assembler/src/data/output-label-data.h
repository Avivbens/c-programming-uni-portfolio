#include <stdio.h>

#include "../constants/constants.h"
#include "../utils/array/array.h"
#include "../utils/list/list.h"
#include "../utils/string/string.h"

#ifndef EXTERN_DATA_H
#define EXTERN_DATA_H

typedef enum { OUTPUT_LABEL_EXTERN, OUTPUT_LABEL_ENTRY } OutputLabelType;

typedef struct {
    String name;
    String* line_addresses;
    OutputLabelType type;
    int size;
} OutputLabel;

LinkedList* get_output_label_list(void);
int add_output_label(String name, String line_address, OutputLabelType type);
void iterate_output_labels(void (*callback)(OutputLabel*, String, FILE*),
                           String context, FILE* exec);

int output_label_list_size(void);
void debug_output_label_labels(void);

#endif
