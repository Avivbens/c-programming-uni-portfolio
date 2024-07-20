#include "macros-data.h"

#include <stdio.h>
#include <stdlib.h>

LinkedList* get_macros_list(void) {
    static LinkedList* macros = NULL;

    if (macros == NULL) {
        macros = create_list();
    }

    return macros;
}

int add_macro(String name, String value) {
    LinkedList* macros = get_macros_list();

    if (has_list(macros, name)) {
        printf("Error: Macro %s already defined\n", name);
        return EXIT_FAILURE;
    }

    insert_list(macros, name, value);

    return EXIT_SUCCESS;
}

String get_macro(String name) {
    LinkedList* macros = get_macros_list();

    return get_list(macros, name);
}

int has_macro(String name) {
    LinkedList* macros = get_macros_list();

    return has_list(macros, name);
}

void debug_macros(void) {
    LinkedList* macros = get_macros_list();

    printf("Macros:\n");
    print_list(macros);
}
