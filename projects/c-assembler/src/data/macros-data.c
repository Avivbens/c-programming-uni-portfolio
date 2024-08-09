#include "macros-data.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * A helper function to print a macro value - string
 */
static void print_string(void* data) { printf("%s", (String)data); }

/**
 * Get the macros list instance
 */
LinkedList* get_macros_list(void) {
    static LinkedList* macros = NULL;

    if (macros == NULL) {
        macros = create_list();
    }

    return macros;
}

/**
 * Add a macro to the list
 *
 * @param name The macro name
 * @param value The macro value
 *
 * @returns int - The result of the operation
 */
int add_macro(String name, String value) {
    int value_size = sizeof(String);
    LinkedList* macros = get_macros_list();

    if (has_list(macros, name)) {
        printf("Error: Macro %s already defined\n", name);
        return EXIT_FAILURE;
    }

    insert_list(macros, name, value, value_size);

    return EXIT_SUCCESS;
}

/**
 * Get a macro from the list
 *
 * @param name The macro name
 *
 * @returns String - The macro value
 */
String get_macro(String name) {
    LinkedList* macros = get_macros_list();

    return (String)get_list(macros, name);
}

/**
 * Check if a macro exists in the list
 *
 * @param name The macro name
 * @returns int - 1 if the macro exists, 0 otherwise
 */
int has_macro(String name) {
    LinkedList* macros = get_macros_list();

    return has_list(macros, name);
}

/**
 * Print the macros list
 * Used for debugging purposes
 */
void debug_macros(void) {
    LinkedList* macros = get_macros_list();

    printf("Macros:\n");
    print_list(macros, print_string);
}
