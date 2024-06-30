#include "macros-parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int register_macro(String file_path) {
    FILE *file;
    char line[256];

    file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Error: Could not open file %s\n", file_path);
        return EXIT_FAILURE;
    }

    while (fgets(line, sizeof(line), file)) {
        /* TODO - extract macro value and name */
        /* char *name = strtok(line, " "); */
        /* char *value = strtok(NULL, "\n"); */

        /* if (name == NULL || value == NULL) { */
        /*     printf("Error: Invalid macro definition\n"); */
        /*     return EXIT_FAILURE; */
        /* } */

        /* TODO - call macros table */
        /* set_insert(macros, name, value); */
    }

    fclose(file);

    return EXIT_SUCCESS;
}
