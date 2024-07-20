
#include "symbols-data.h"

#include <stdio.h>
#include <stdlib.h>

HashTable* get_symbols_table(void) {
    static HashTable* symbolsTable = NULL;

    if (symbolsTable == NULL) {
        symbolsTable = create_table();
    }

    return symbolsTable;
}

int add_label(String name, Symbol value) {
    HashTable* symbolsTable = get_symbols_table();

    if (has_table(symbolsTable, name)) {
        printf("Error: label %s already defined\n", name);
        return EXIT_FAILURE;
    }
    /*Reminder: add a test external label before insert*/

    insert_table(symbolsTable, name, value);

    return EXIT_SUCCESS;
}

Symbol get_label(String name) {
    HashTable* symbolsTable = get_symbols_table();

    return get_table(symbolsTable, name);
}

int has_label(String name) {
    HashTable* symbolsTable = get_symbols_table();

    return has_table(symbolsTable, name);
}
