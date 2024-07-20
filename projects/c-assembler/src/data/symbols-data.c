#include "symbols-data.h"

#include <stdio.h>
#include <stdlib.h>

LinkedList* get_symbols_list(void) {
    static LinkedList* symbolsTable = NULL;

    if (symbolsTable == NULL) {
        symbolsTable = create_list();
    }

    return symbolsTable;
}

int add_label(String name, Symbol value) {
    LinkedList* symbolsTable = get_symbols_list();

    if (has_list(symbolsTable, name)) {
        printf("Error: label %s already defined\n", name);
        return EXIT_FAILURE;
    }
    /*Reminder: add a test external label before insert*/

    insert_list(symbolsTable, name, value);

    return EXIT_SUCCESS;
}

Symbol get_label(String name) {
    LinkedList* symbolsTable = get_symbols_list();

    return get_list(symbolsTable, name);
}

int has_label(String name) {
    LinkedList* symbolsTable = get_symbols_list();

    return has_list(symbolsTable, name);
}
