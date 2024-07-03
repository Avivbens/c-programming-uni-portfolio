#include "macros-parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../data/macros-data.h"

/**
 * First iteration over a file, assigning macro values to their names.
 * If a macro already exists, it would log an error and return EXIT_FAILURE.
 */
int macro_registration(String file_path) {
    FILE *file;
    int start_macro = 0;
    int func_bool;

    String last_space;

    int exit_code = EXIT_SUCCESS;

    String macro_name = (String)malloc(MAX_LINE_LENGTH);
    String macro_value = (String)malloc(MAX_MACRO_SIZE);

    char line[MAX_LINE_LENGTH];

    if (macro_name == NULL || macro_value == NULL) {
        fprintf(stderr,
                "Error: Could not allocate memory for macro name or value\n");
        exit(EXIT_FAILURE);
    }

    file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Error: Could not open file %s\n", file_path);
        return EXIT_FAILURE;
    }

    while (fgets(line, sizeof(line), file)) {
        /**
         * Start reading macro values
         */
        if (strstr(line, MACRO_TAG_OPEN) != NULL) {
            start_macro = 1;

            last_space = strrchr(line, ' ');
            macro_name = replace_substring(strdup(last_space + 1),
                                           (String) "\n", (String) "");

            if (macro_name == NULL) {
                printf("Error: Could not allocate memory for macro name\n");
                exit(EXIT_FAILURE);
            }

            continue;
        }

        /**
         * Stop reading macro values
         */
        if (strstr(line, MACRO_TAG_CLOSE) != NULL) {
            start_macro = 0;

            /**
             * Check if macro already exists
             */
            func_bool = has_macro(macro_name);
            if (func_bool) {
                printf("Error: Macro %s already exists\n", macro_name);
                exit_code = EXIT_FAILURE;
            } else {
                add_macro(macro_name, macro_value);
            }

            /**
             * Clear macro values
             */
            memset(macro_name, 0, MAX_LINE_LENGTH);
            memset(macro_value, 0, MAX_MACRO_SIZE);

            continue;
        }

        if (!start_macro) {
            continue;
        }

        /**
         * Insert macro values
         */
        snprintf(macro_value, MAX_MACRO_SIZE, "%s%s", macro_value, line);
    }

    /**
     * Check if a macro is not closed
     */
    if (start_macro) {
        printf("Error: Macro %s is not closed\n", macro_name);
        exit_code = EXIT_FAILURE;
    }

    fclose(file);

    return exit_code;
}
