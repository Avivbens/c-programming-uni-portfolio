#include "macros-parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define strdup _strdup
#endif

/**
 * Check if a line is a macro definition
 */
static int is_open_macro(String line) {
    return strstr(line, MACRO_TAG_OPEN) != NULL;
}

/**
 * Check if a line is a macro closing tag
 */
static int is_close_macro(String line) {
    return strstr(line, MACRO_TAG_CLOSE) != NULL;
}

/**
 * Trim the line value and try to match it with a macro name
 *
 * @attention - free this memory after use
 */
static String extract_macro_name(String line) {
    String trimmed = trim_string(line);
    return trimmed;
}

/**
 * Create a post-processed file based on the registered macros
 *
 * @attention - free this memory after use
 *
 * @throw In case of an error, it would return NULL
 * @returns post-processed file path
 */
static String create_post_processed_file(String file_name) {
    FILE *file;
    FILE *post_processed_file;

    char line[MAX_LINE_LENGTH];
    int exit_code = EXIT_SUCCESS;
    int start_macro = 0;

    String macro_name = (String)malloc(MAX_LINE_LENGTH);
    String macro_value;

    String original_file_path = get_file_name_with_extension(
        file_name, (String)ORIGINAL_FILE_EXTENSION);

    String post_processed_file_path = generate_dist_path_for_file(
        file_name, (String)POST_PROCESS_FILE_EXTENSION);

    if (macro_name == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for macro name\n");
        exit(EXIT_FAILURE);
    }

    /**
     * Open and create files
     */
    file = fopen(original_file_path, "r");
    if (file == NULL) {
        printf("Error: Could not open file %s\n", file_name);
        exit(EXIT_FAILURE);
    }

    free(original_file_path);
    original_file_path = NULL;

    post_processed_file = fopen(post_processed_file_path, "w");
    if (post_processed_file == NULL) {
        printf("Error: Could not create post-process file %s\n",
               post_processed_file_path);
        exit(EXIT_FAILURE);
    }

    /**
     * Read file line by line
     */
    while (fgets(line, sizeof(line), file)) {
        /**
         * Apply macros if exists
         */
        if (is_open_macro(line)) {
            start_macro = 1;
            continue;
        }

        /**
         * Stop skipping macros definitions
         */
        if (is_close_macro(line)) {
            start_macro = 0;
            continue;
        }

        /**
         * Skip macros definitions
         */
        if (start_macro) {
            continue;
        }

        /**
         * Replace macros with their values
         */
        macro_name = extract_macro_name(line);
        if (!has_macro(macro_name)) {
            /**
             * Write source file line to post-processed file
             */
            fprintf(post_processed_file, "%s", line);
            free(macro_name);
            macro_name = NULL;

            continue;
        }

        macro_value = get_macro(macro_name);
        if (macro_value == NULL) {
            printf("Error: Macro '%s' does not exist\n", macro_name);
            exit_code = EXIT_FAILURE;
        }

        /**
         * Write macro value to post-processed file
         */
        fprintf(post_processed_file, "%s", macro_value);

        free(macro_name);
        macro_name = NULL;
    }

    fclose(file);
    fclose(post_processed_file);

    if (exit_code == EXIT_FAILURE) {
        return NULL;
    }

    return post_processed_file_path;
}

/**
 * First iteration over a file, assigning macro values to their names.
 * If a macro already exists, it would log an error and return EXIT_FAILURE.
 *
 * @throw In case of an error, it would return EXIT_FAILURE
 * @returns EXIT_SUCCESS if all macros were registered successfully
 */
static int macro_registration(String file_name) {
    FILE *file;
    int start_macro = 0;
    String last_space;

    int exit_code = EXIT_SUCCESS;
    int exit_code_temp;

    String file_path =
        get_file_name_with_extension(file_name, ORIGINAL_FILE_EXTENSION);
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
        printf("Error: Could not open file %s\n", file_name);
        return EXIT_FAILURE;
    }

    free(file_path);
    file_path = NULL;

    /**
     * Read file line by line
     */
    while (fgets(line, sizeof(line), file)) {
        /**
         * Start reading macro values
         */
        if (is_open_macro(line)) {
            start_macro = 1;

            last_space = strrchr(line, ' ');
            macro_name =
                replace_substring(last_space + 1, (String) "\n", (String) "");

            if (macro_name == NULL) {
                printf("Error: Could not allocate memory for macro name\n");
                exit(EXIT_FAILURE);
            }

            continue;
        }

        /**
         * Stop reading macro values
         */
        if (is_close_macro(line)) {
            start_macro = 0;
            exit_code_temp = add_macro(macro_name, macro_value);
            exit_code =
                exit_code_temp == EXIT_FAILURE ? EXIT_FAILURE : exit_code;

            free(macro_name);
            macro_name = NULL;

            free(macro_value);
            macro_value = NULL;

            continue;
        }

        if (!start_macro) {
            continue;
        }

        /**
         * Insert macro values
         */
        strcat(macro_value, line);
    }

    /**
     * Check if a macro is not closed
     */
    if (start_macro) {
        printf("Error: Macro %s is not closed\n", macro_name);
        exit_code = EXIT_FAILURE;
    }

    fclose(file);

    free(macro_name);
    macro_name = NULL;

    free(macro_value);
    macro_value = NULL;

    return exit_code;
}

/**
 * Register and create post-processed files
 *
 * @param file_names the original files paths without the file extension
 *
 * @attention - free this memory after use (array and each element)
 *
 * @throw In case of an error, it would exit the program with EXIT_FAILURE
 * @returns post-processed files paths
 */
String *handle_macros(String *file_names) {
    int i;
    int is_failed = EXIT_SUCCESS;
    int macro_reg_res = EXIT_SUCCESS;

    String post_processed_file_path;
    String *post_processed_files = (String *)malloc(MAX_PATH_LENGTH);

    if (post_processed_files == NULL) {
        fprintf(stderr,
                "Error: Could not allocate memory for post-processed files\n");
        exit(EXIT_FAILURE);
    }

    /**
     * Register macros
     */
    for (i = 0; file_names[i] != NULL; i++) {
        macro_reg_res = macro_registration(file_names[i]);

        if (macro_reg_res == EXIT_FAILURE) {
            is_failed = EXIT_FAILURE;
        }
    }

    if (is_failed) {
        printf("Error: Could not register macros\n");
        exit(EXIT_FAILURE);
    }

    /**
     * Create post-processed files
     */
    for (i = 0; file_names[i] != NULL; i++) {
        post_processed_file_path = create_post_processed_file(file_names[i]);

        if (post_processed_file_path == NULL) {
            printf("Error: Could not create post-processed file\n");
            exit(EXIT_FAILURE);
        }

        post_processed_files[i] = post_processed_file_path;
    }

    return post_processed_files;
}
