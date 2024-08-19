#include "extern-data.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Get the externs list instance
 */
LinkedList* get_extern_list(void) {
    static LinkedList* extern_list = NULL;

    if (extern_list == NULL) {
        extern_list = create_list();
    }

    return extern_list;
}

/**
 * Add an extern label to the externs list
 *
 * @param name The name of the extern label
 * @param line_address The line address of the extern label
 *
 * @returns EXIT_SUCCESS if the label was added successfully, EXIT_FAILURE
 * otherwise
 */
int add_extern(String name, String line_address) {
    int value_size = sizeof(ExternLabel);
    LinkedList* externs = get_extern_list();
    ExternLabel* new_extern;
    ExternLabel* existing_extern;
    String* new_line_addresses;

    int i;

    /**
     * Register a new extern in the externs list
     */
    if (!has_list(externs, name)) {
        new_extern = (ExternLabel*)malloc(value_size);
        if (new_extern == NULL) {
            printf("Error: failed to allocate memory for extern label %s\n",
                   name);
            return EXIT_FAILURE;
        }

        new_line_addresses = (String*)malloc(sizeof(String) * 1);
        if (new_line_addresses == NULL) {
            printf("Error: failed to allocate memory for extern label %s\n",
                   name);
            return EXIT_FAILURE;
        }

        new_extern->line_addresses = new_line_addresses;
        new_extern->line_addresses[0] = strdup(line_address);
        new_extern->size = 1;

        new_extern->name = strdup(name);

        insert_list(externs, name, new_extern, value_size);
        return EXIT_SUCCESS;
    }

    existing_extern = (ExternLabel*)get_list(externs, name);

    /**
     * Add the line address to the list of line addresses
     */

    new_line_addresses =
        (String*)malloc(sizeof(String) * (existing_extern->size + 1));
    if (new_line_addresses == NULL) {
        printf("Error: failed to allocate memory for extern label %s\n", name);
        return EXIT_FAILURE;
    }

    for (i = 0; i < existing_extern->size; i++) {
        new_line_addresses[i] = strdup(existing_extern->line_addresses[i]);
    }

    new_line_addresses[existing_extern->size] = strdup(line_address);
    existing_extern->size++;

    free(existing_extern->line_addresses);
    existing_extern->line_addresses = new_line_addresses;

    return EXIT_SUCCESS;
}

/**
 * Iterate over the externs list
 */
void iterate_externs(void (*callback)(ExternLabel*, String, FILE*),
                     String context, FILE* exec) {
    LinkedList* extern_list = get_extern_list();

    iterate_list(extern_list, (void (*)(void*, String, FILE*))callback, context,
                 exec);
}

/**
 * Get the size of the externs list
 */
int extern_list_size(void) {
    LinkedList* extern_list = get_extern_list();

    return get_list_size(extern_list);
}

static void print_extern(void* data) {
    int i;
    ExternLabel* extern_label = (ExternLabel*)data;

    printf("Name: %s\n", extern_label->name);
    for (i = 0; i < extern_label->size; i++) {
        printf("Line Address: %s\n", extern_label->line_addresses[i]);
    }
}

/**
 * Print the extern labels
 */
void debug_extern_labels(void) {
    LinkedList* extern_list = get_extern_list();

    printf("Extern Labels:\n");
    print_list(extern_list, print_extern);
}
