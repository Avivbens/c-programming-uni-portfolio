#include "macros-data.h"

#include <stdio.h>
#include <stdlib.h>

#include "../utils/table/table.h"

static HashTable *macros = NULL;

static void init_macros(void) {
    if (macros != NULL) {
        return;
    }

    macros = create_table();
}

int add_macro(String name, String value) {
    init_macros();

    if (has_table(macros, name)) {
        printf("Error: Macro %s already defined\n", name);
        return EXIT_FAILURE;
    }

    insert_table(macros, name, value);

    return EXIT_SUCCESS;
}

String get_macro(String name) {
    init_macros();

    return get_table(macros, name);
}

int has_macro(String name) {
    init_macros();

    return has_table(macros, name);
}

void debug_macros(void) {
    init_macros();

    printf("Macros:\n");
    print_table(macros);
}
