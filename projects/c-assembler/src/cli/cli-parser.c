#include "cli-parser.h"

#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define strdup _strdup
#endif

/**
 * Get the files names from the command line arguments
 * File names are not include the file extension
 *
 * @attention - free this memory after use (array and each element)
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
 *
 * @attention - free this memory after use
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
 * Generate the path for the post-processed file
 *
 * @attention - free this memory after use
 *
 * @param file_path the original file path
 * @param extension the extension for the post-processed file
 *
 * @return the path for the dist file
 */
String generate_dist_path_for_file(String file_path, String extension) {
    String dir_name = dirname(file_path);
    String file_name = basename(file_path);

    String target = (String)malloc(MAX_PATH_LENGTH * sizeof(char));

    if (target == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    strcpy(target, dir_name);
    strcat(target, "/../dist/");
    strcat(target, file_name);
    strcat(target, extension);

    return target;
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

        free(file_path);
        file_path = NULL;
    }

    if (error != EXIT_SUCCESS) {
        exit(error);
    }
}
