#include "labels-parser.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * function to determine if a string is a label
 *
 * a label must:
 * start with an alphabet character(upper or lower)
 * followed by alphabets characters and or digits only
 * the max length is 31 characters
 * The last character must be ':' (and be max in the 32th index)
 * no spaces allowed at all
 *
 * @returns 1 if the string is a label, 0 otherwise
 */
static int is_label(String str) {
    int i;
    int len = strlen(str);
    if (len > MAX_LABEL_LENGTH + 1) {
        return 0;
    }

    if (!isalpha(str[0])) {
        return 0;
    }

    if (str[len - 1] != ':') {
        return 0;
    }

    for (i = 1; i < len - 1; i++) {
        if (!isalnum(str[i])) {
            return 0;
        }
    }

    return 1;
}

/**
 * First iteration over a file, Locating the labels and entering the names in
 * the list. At this stage we will initialize the values ​​of all the
 * labels to 0. If a label already exists, it would log an error and return
 * EXIT_FAILURE.
 *
 * @throw In case of an error, it would return EXIT_FAILURE
 * @returns EXIT_SUCCESS if all macros were registered successfully
 */
static int label_registration(String file_name) {
    FILE *file;
    int exit_code = EXIT_SUCCESS;
    char line[MAX_LINE_LENGTH];
    String file_path = get_file_name_with_extension(
        file_name, (String)ORIGINAL_FILE_EXTENSION);
    String label_name = (String)malloc(MAX_LABEL_LENGTH);

    if (label_name == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for label name\n");
        exit(EXIT_FAILURE);
    }

    file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Error: Could not open file %s\n", file_name);
        return EXIT_FAILURE;
    }

    while (fgets(line, sizeof(line), file)) {
        String first_word = get_first_word_from_line(line);
        int is_word_label = is_label(first_word);
        if (is_word_label == 0) {
            continue;
        }

        /**
         * check if the word if not already in the symbol list or extern list
         * if yes - exit with error
         * if not - insert
         */
        if (!has_label(first_word)) {
            printf("label %s already exits", first_word);
            exit_code = EXIT_FAILURE;
        } else {
            add_label(first_word, (Label){first_word, 0, 0, 0});
        }
    }

    return exit_code;
}

static int label_fill(String file_name) {
    FILE *file;
    int exit_code = EXIT_SUCCESS;
    char line[MAX_LINE_LENGTH];
    String file_path = get_file_name_with_extension(
        file_name, (String)ORIGINAL_FILE_EXTENSION);

    int word_counter = 0;

    String label_name = (String)malloc(MAX_LABEL_LENGTH);
    if (label_name == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for label name\n");
        exit(EXIT_FAILURE);
    }

    file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Error: Could not open file %s\n", file_name);
        return EXIT_FAILURE;
    }

    while (fgets(line, sizeof(line), file)) {
        String first_word = get_first_word_from_line(line);
        int is_word_label = is_label(first_word);
        if (is_word_label == 0) {
            word_counter++;
            continue;
        }

        /**
         * Aviv, I think we need to add a function that knows how to update data
         * in an existing list to the list file if it is possible to use one
         * of the existing functions, please update me and I will correct
         * accordingly
         */
        update_list(symbolsTable, first_word, instCounter + word_counter);
    }
}

static int fill_symbols_list(String *file_names) {
    int i;
    for (i = 0; file_names[i] != NULL; i++) {
        label_fill(file_names[i]);
    }
    instCounter += SYMBOL_START_POINT;
    return FILL_SUCCESS;
}

void *handle_labels(String *file_names) {
    int i;
    int is_failed = EXIT_SUCCESS;
    int label_reg_res = EXIT_SUCCESS;
    int label_fill_res = EXIT_SUCCESS;

    /**
     * Register labels
     */
    for (i = 0; file_names[i] != NULL; i++) {
        label_reg_res = label_registration(file_names[i]);

        if (label_reg_res == EXIT_FAILURE) {
            is_failed = EXIT_FAILURE;
        }
    }

    if (is_failed) {
        printf("Error: Could not register labels\n");
        exit(EXIT_FAILURE);
    }

    /**
     * Adding the address's value in memory
     */
    for (i = 0; file_names[i] != NULL; i++) {
        label_fill_res = fill_symbols_list(file_names[i]);

        if (label_fill_res == NULL) {
            printf("Error: Could not fill the values of the labels\n");
            exit(EXIT_FAILURE);
        }
    }
}
