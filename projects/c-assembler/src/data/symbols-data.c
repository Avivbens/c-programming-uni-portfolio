#include "symbols-data.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * A helper function to print a label - Label
 */
static void print_label(void* data) {
    Label* label = (Label*)data;

    printf("Name: %s\n", label->name);
    printf("Value: %s\n", label->value);
    printf("Exists: %d\n", label->exists);
    printf("ExistsExtern: %d\n", label->existsExtern);
}

/**
 * Get the labels list instance
 */
LinkedList* get_labels_list(void) {
    static LinkedList* symbolsTable = NULL;

    if (symbolsTable == NULL) {
        symbolsTable = create_list();
    }

    return symbolsTable;
}

/**
 * Add a label to the list
 *
 * @param name The label name
 * @param value A pointer to a label value
 *
 * @returns int - The result of the operation
 */
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

/**
 * Get a label from the list
 *
 * @param name The label name
 *
 * @returns - A pointer to the label value
 */
Label* get_label(String name) {
    LinkedList* symbolsTable = get_labels_list();

    return (Label*)get_list(symbolsTable, name);
}

/**
 * Check if a label exists in the list
 *
 * @param name The label name
 * @returns int - 1 if the label exists, 0 otherwise
 */
int has_label(String name) {
    LinkedList* symbolsTable = get_labels_list();

    return has_list(symbolsTable, name);
}

/**
 * Print the labels list
 * Used for debugging purposes
 */
void debug_labels(void) {
    LinkedList* macros = get_labels_list();

    printf("Labels:\n");
    print_list(macros, print_label);
}
