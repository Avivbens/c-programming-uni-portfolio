#include <stdio.h>

#include "../constants/constants.h"
#include "../utils/array/array.h"
#include "../utils/list/list.h"
#include "../utils/string/string.h"

#ifndef EXTERN_DATA_H
#define EXTERN_DATA_H

typedef struct {
    String name;
    String* line_addresses;
    int size;
} ExternLabel;

LinkedList* get_extern_list(void);
int add_extern(String name, String line_address);

void iterate_externs(void (*callback)(ExternLabel*, String, FILE*),
                     String context, FILE* exec);

int extern_list_size(void);
void debug_extern_labels(void);

#endif
