#include "output-label-data.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define strdup _strdup
#endif

/**
 * Get the output labels list instance
 */
LinkedList* get_output_label_list(void) {
    static LinkedList* output_label_list = NULL;

    if (output_label_list == NULL) {
        output_label_list = create_list();
    }

    return output_label_list;
}

/**
 * Add an output label label to the output_labels list
 *
 * @param name The name of the output_label label
 * @param line_address The line address of the output_label label
 *
 * @returns EXIT_SUCCESS if the label was added successfully, EXIT_FAILURE
 * otherwise
 */
int add_output_label(String name, String line_address, OutputLabelType type) {
    int value_size = sizeof(OutputLabel);
    LinkedList* output_labels = get_output_label_list();
    OutputLabel* new_output_label;
    OutputLabel* existing_output_label;
    String* new_line_addresses;

    int i;

    /**
     * Register a new output_label in the output_labels list
     */
    if (!has_list(output_labels, name)) {
        new_output_label = (OutputLabel*)malloc(value_size);
        if (new_output_label == NULL) {
            printf(
                "Error: failed to allocate memory for output_label label %s\n",
                name);
            return EXIT_FAILURE;
        }

        new_line_addresses = (String*)malloc(sizeof(String) * 1);
        if (new_line_addresses == NULL) {
            printf(
                "Error: failed to allocate memory for output_label label %s\n",
                name);
            return EXIT_FAILURE;
        }

        new_output_label->line_addresses = new_line_addresses;
        new_output_label->line_addresses[0] = strdup(line_address);
        new_output_label->size = 1;
        new_output_label->type = type;

        new_output_label->name = strdup(name);

        insert_list(output_labels, name, new_output_label, value_size);
        return EXIT_SUCCESS;
    }

    existing_output_label = (OutputLabel*)get_list(output_labels, name);

    /**
     * Add the line address to the list of line addresses
     */
    new_line_addresses =
        (String*)malloc(sizeof(String) * (existing_output_label->size + 1));
    if (new_line_addresses == NULL) {
        printf("Error: failed to allocate memory for output_label label %s\n",
               name);
        return EXIT_FAILURE;
    }

    for (i = 0; i < existing_output_label->size; i++) {
        new_line_addresses[i] =
            strdup(existing_output_label->line_addresses[i]);
    }

    new_line_addresses[existing_output_label->size] = strdup(line_address);
    existing_output_label->size++;

    free(existing_output_label->line_addresses);
    existing_output_label->line_addresses = new_line_addresses;

    return EXIT_SUCCESS;
}

/**
 * Iterate over the output_labels list
 */
void iterate_output_labels(void (*callback)(OutputLabel*, String, FILE*),
                           String context, FILE* exec) {
    LinkedList* output_label_list = get_output_label_list();

    iterate_list(output_label_list, (void (*)(void*, String, FILE*))callback,
                 context, exec);
}

/**
 * Get the size of the output_labels list
 */
int output_label_list_size(void) {
    LinkedList* output_label_list = get_output_label_list();

    return get_list_size(output_label_list);
}

static void print_output_label(void* data) {
    int i;
    OutputLabel* output_label_label = (OutputLabel*)data;

    printf("Name: %s\n", output_label_label->name);
    for (i = 0; i < output_label_label->size; i++) {
        printf("Line Address: %s\n", output_label_label->line_addresses[i]);
    }
}

/**
 * Print the output_label labels
 */
void debug_output_label_labels(void) {
    LinkedList* output_label_list = get_output_label_list();

    printf("Output_label Labels:\n");
    print_list(output_label_list, print_output_label);
}

/**
 * Check if an output_label exists in the list
 *
 * @param name The output_label name
 */
int has_output_label(String name) {
    LinkedList* symbols_list = get_output_label_list();

    return has_list(symbols_list, name);
}
