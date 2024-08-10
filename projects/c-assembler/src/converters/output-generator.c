#include "output-generator.h"

#include <stdio.h>
#include <stdlib.h>

static int generate_file_output(String file_path) {
    FILE *file;
    int exit_code = EXIT_SUCCESS;
    char line[MAX_LINE_LENGTH];
    int line_number = 0;

    file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Error: Could not open file '%s'\n", file_path);
        return EXIT_FAILURE;
    }
}

/**
 * Handle the output for the files
 *
 * @param file_paths the paths for the files
 *
 * @return EXIT_SUCCESS if the output was successful, otherwise EXIT_FAILURE
 */
int handle_output(String *file_paths) {
    int i;
    int is_failed = EXIT_SUCCESS;
    int create_output_res = EXIT_SUCCESS;

    /**
     * Create output files
     */
    for (i = 0; file_paths[i] != NULL; i++) {
        create_output_res = generate_file_output(file_paths[i]);

        if (create_output_res == EXIT_FAILURE) {
            printf("Error: Could not create output for file: '%s' \n",
                   file_paths[i]);

            is_failed = EXIT_FAILURE;
        }
    }

    return is_failed;
}
