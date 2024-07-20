#include "symbols-data.h"

#include <stdio.h>
#include <stdlib.h>

static void print_label(void* data) {
    Label* symbol = (Label*)data;

    printf("Name: %s\n", symbol->name);
    printf("Value: %s\n", symbol->value);
    printf("Exists: %d\n", symbol->exists);
    printf("ExistsExtern: %d\n", symbol->existsExtern);
}

LinkedList* get_labels_list(void) {
    static LinkedList* symbolsTable = NULL;

    if (symbolsTable == NULL) {
        symbolsTable = create_list();
    }

    return symbolsTable;
}

int add_label(String name, Label* value) {
    int value_size = sizeof(Label);
    LinkedList* symbols = get_labels_list();

    if (has_list(symbols, name)) {
        printf("Error: label %s already defined\n", name);
        return EXIT_FAILURE;
    }

    insert_list(symbols, name, value, value_size);

    return EXIT_SUCCESS;
}

Label* get_label(String name) {
    LinkedList* symbolsTable = get_labels_list();

    return (Label*)get_list(symbolsTable, name);
}

int has_label(String name) {
    LinkedList* symbolsTable = get_labels_list();

    return has_list(symbolsTable, name);
}

void debug_labels(void) {
    LinkedList* macros = get_labels_list();

    printf("Labels:\n");
    print_list(macros, print_label);
}
