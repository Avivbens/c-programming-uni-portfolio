#include "macros-data.h"

#include <stdio.h>
#include <stdlib.h>

HashTable* get_macros_table(void) {
    static HashTable* macros = NULL;

    if (macros == NULL) {
        macros = create_table();
    }

    return macros;
}

int add_macro(String name, String value) {
    HashTable* macros = get_macros_table();

    if (has_table(macros, name)) {
        printf("Error: Macro %s already defined\n", name);
        return EXIT_FAILURE;
    }

    insert_table(macros, name, value);

    return EXIT_SUCCESS;
}

String get_macro(String name) {
    HashTable* macros = get_macros_table();

    return get_table(macros, name);
}

int has_macro(String name) {
    HashTable* macros = get_macros_table();

    return has_table(macros, name);
}

void debug_macros(void) {
    HashTable* macros = get_macros_table();

    printf("Macros:\n");
    print_table(macros);
}
