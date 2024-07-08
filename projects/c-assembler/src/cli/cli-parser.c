#include "./cli-parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Get the files names from the command line arguments
 * File names are not include the file extension
 */
String *get_files_names(int argc, String *argv) {
    String *files = (String *)malloc(argc * sizeof(String));
    int i = 1;

    if (files == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    for (; i < argc; i++) {
        files[i - 1] = (String)malloc(MAX_PATH_LENGTH * sizeof(char));

        if (files[i - 1] == NULL) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        strcpy(files[i - 1], argv[i]);
    }

    files[argc - 1] = NULL;

    return files;
}

/**
 * Get the file name with the original source file extension
 */
String get_file_name_with_extension(String file_name, String extension) {
    String file = (String)malloc(MAX_PATH_LENGTH * sizeof(char));

    if (file == NULL) {
        fprintf(
            stderr,
            "Error: get_file_name_with_extension - Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    strcpy(file, file_name);
    strcat(file, extension);

    return file;
}

/**
 * Verify all files exists
 * If a file does not exist, the program will exit with EXIT_FAILURE
 */
void verify_files_exists(String *files) {
    int i = 0;
    int error = EXIT_SUCCESS;
    String file_path;

    for (; files[i] != NULL; i++) {
        file_path =
            get_file_name_with_extension(files[i], ORIGINAL_FILE_EXTENSION);

        if (!is_file_exists(file_path)) {
            fprintf(stderr, "Error: File %s does not exist\n", files[i]);
            error = EXIT_FAILURE;
        }
    }

    if (error != EXIT_SUCCESS) {
        exit(error);
    }
}
