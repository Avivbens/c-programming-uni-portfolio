#include "symbols-data.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * The Instruction Counter begins at 100 and increases by the number of memory
 * cells each instruction uses.
 *
 * This indicates the next available memory cell.
 */
int get_instruction_counter(int increment) {
    static int instruction_counter = 0;
    int res = instruction_counter;

    instruction_counter += increment;
    return res;
}

/**
 * The Data Counter serves a similar purpose as the Instruction Counter but is
 * specifically for data.
 *
 * For safety reasons, they are kept separate.
 */
int get_data_counter(int increment) {
    static int data_counter = 0;
    int res = data_counter;

    data_counter += increment;
    return res;
}

/**
 * A helper function to print a label - Label
 */
static void print_label(void* data) {
    Label* label = (Label*)data;

    printf("Is Defined: %d\n", label->is_defined);
    printf("Memory address: %d\n", label->memory_address);

    printf("Has Entry: %d\n", label->has_entry);
    printf("Has Extern: %d\n", label->has_extern);
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
 * @returns EXIT_SUCCESS if the label was added successfully, EXIT_FAILURE
 * otherwise
 */
int add_label(String name, LabelType type, int memory_address) {
    int value_size = sizeof(Label);
    LinkedList* labels = get_labels_list();
    Label* new_label;
    Label* existing_label;
    int should_not_define = type == LABEL_EXTERN || type == LABEL_ENTRY;

    /**
     * Register a new label in the symbols table
     */
    if (!has_list(labels, name)) {
        new_label = (Label*)malloc(value_size);
        if (new_label == NULL) {
            printf("Error: failed to allocate memory for label %s\n", name);
            return EXIT_FAILURE;
        }

        new_label->memory_address = should_not_define ? 0 : memory_address;
        new_label->is_defined = should_not_define ? 0 : 1;

        new_label->has_extern = type == LABEL_EXTERN ? 1 : 0;
        new_label->has_entry = type == LABEL_ENTRY ? 1 : 0;

        insert_list(labels, name, new_label, value_size);
        return EXIT_SUCCESS;
    }

    existing_label = (Label*)get_list(labels, name);

    /**
     * No need to change data if the label is extern or entry
     */
    if (type == LABEL_EXTERN) {
        existing_label->has_extern = 1;
        return EXIT_SUCCESS;
    }

    if (type == LABEL_ENTRY) {
        existing_label->has_entry = 1;
        return EXIT_SUCCESS;
    }

    /**
     * Prevent redefining a label
     */
    if (existing_label->is_defined == 1) {
        printf("Error: label %s already defined\n", name);
        return EXIT_FAILURE;
    }

    /**
     * Update the label if it already exists
     */
    existing_label->is_defined = 1;
    existing_label->memory_address = memory_address;
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
    LinkedList* symbolsTable = get_labels_list();

    printf("Labels:\n");
    print_list(symbolsTable, print_label);
}
