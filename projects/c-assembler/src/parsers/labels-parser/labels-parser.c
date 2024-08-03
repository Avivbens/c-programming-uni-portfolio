#include "labels-parser.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @param line - the line to check
 *
 * @returns:
 * 0 - if the label is not a label type
 * 1 - if the label is an external label
 * 2 - if the label is an entry label
 * 3 - if the label is a data label
 * 4 - if the label is a string label
 */
static LabelType get_label_type(String line) {
    String second_word;

    if (starts_with(line, (String)LABEL_EXTERN_PREFIX)) {
        return LABEL_EXTERN;
    }

    if (starts_with(line, (String)LABEL_ENTRY_PREFIX)) {
        return LABEL_ENTRY;
    }

    second_word = get_word(line, 1);
    if (strcmp(second_word, LABEL_DATA_PREFIX)) {
        return LABEL_DATA;
    }

    if (strcmp(second_word, LABEL_STRING_PREFIX)) {
        return LABEL_STRING;
    }

    return NOT_LABEL_TYPE;
}

/**
 * @param name - the name of the potential label
 * @returns 1 if the string is a label, 0 otherwise
 *
 * @note
 * This function only validate that the label name itself is valid
 */
static int is_label_name_ok(String name) {
    int i;
    int len = strlen(name);

    if (len > MAX_LABEL_LENGTH) {
        return 0;
    }

    for (i = 0; i < len - 1; i++) {
        if (!isalnum(name[i])) {
            return 0;
        }
    }

    return 1;
}

/**
 * @param name - the name of the potential label
 *
 * @returns 1 if the label is allowed, 0 otherwise
 *
 * @note
 * We cannot use operators names, register names, or command names, macro names,
 * etc as labels
 */
static int is_label_name_allowed(String name) {
    if (!is_label_name_ok(name)) {
        return 0;
    }

    /* Cannot be a macro name */
    if (has_macro(name)) {
        printf(
            "Error: label '%s' name is not valid- already defined as a macro\n",
            name);
        return 0;
    }

    /* Cannot be a native operand */
    if (is_command(name)) {
        printf(
            "Error: label '%s' name is not valid- already defined as a command "
            "name\n",
            name);
        return 0;
    }

    /* Cannot be a register */
    if (is_register(name)) {
        printf(
            "Error: label '%s' name is not valid- already defined as a "
            "register name\n",
            name);
        return 0;
    }

    return 1;
}

/**
 * @param name - the name of the potential label
 * @returns the proper LabelType
 *
 * @note
 * A label is a string that starts with a letter or a dot, followed by a string
 * Cannot start by a digit, and cannot contain any special characters
 */
static LabelType is_label(String line) {
    LabelType label_type = get_label_type(line);
    String label_name;
    String helper;

    if (label_type == NOT_LABEL) {
        return NOT_LABEL;
    }

    /**
     * In entry and extern labels, the label name is the second word in the line
     */
    if (label_type == LABEL_EXTERN || label_type == LABEL_ENTRY) {
        label_name = get_word(line, 1);

        if (!is_label_name_allowed(label_name)) {
            return LABEL_VIOLATION;
        }

        return label_type;
    }

    /**
     * In data and string labels, the label name is first word in the line,
     * followed by `:`
     */
    label_name = get_word(line, 0);
    if (!ends_with(label_name, (String) ":")) {
        return NOT_LABEL;
    }

    helper = replace_substring(strdup(label_name), (String) ":", (String) "");
    if (!is_label_name_allowed(helper)) {
        return LABEL_VIOLATION;
    }

    return label_type;
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

    String first_word;
    String second_word;

    LabelType label_type;

    String file_path = get_file_name_with_extension(
        file_name, (String)POST_PROCESS_FILE_EXTENSION);

    file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Error: Could not open file %s\n", file_name);
        return EXIT_FAILURE;
    }

    /**
     * Read file line by line
     */
    while (fgets(line, sizeof(line), file)) {
        label_type = is_label(trim_string(line));

        switch ((int)label_type) {
            case LABEL_VIOLATION:
                printf("Error: violation with labels system!\n");
                exit_code = EXIT_FAILURE;
                continue;

            case NOT_LABEL:
                continue;

            case NOT_LABEL_TYPE:

                /**
                 * TODO - apply this logic for each of the label types
                 */
                second_word = get_word(line, 1);
                if (strcmp(second_word, LABEL_EXTERN_PREFIX) == 0) {
                    printf(
                        "WARNING':A label defined at the beginning of the "
                        "extern line is invalid\n");
                    continue;
                }

                if (strcmp(second_word, LABEL_ENTRY_PREFIX) == 0) {
                    printf(
                        "WARNING':A label defined at the beginning of the "
                        "entry line is invalid\n");
                    continue;
                }

                /**
                 * TODO - implement
                 */

            case LABEL_EXTERN:

                /**
                 * TODO - implement
                 */

            case LABEL_ENTRY:
                /**
                 * TODO - implement
                 */

            case LABEL_DATA:
                /**
                 * TODO - implement
                 */

            case LABEL_STRING:
                /**
                 * TODO - implement
                 */
        }
    }

    return exit_code;
}

void *handle_labels(String *file_names) {
    int i;
    int is_failed = EXIT_SUCCESS;
    int label_reg_res = EXIT_SUCCESS;
    /* int label_fill_res = EXIT_SUCCESS; */

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
    /*  for (i = 0; file_names[i] != NULL; i++) {
        label_fill_res = label_fill(file_names[i]);

        if (label_fill_res == NULL) {
            is_failed = EXIT_FAILURE;
        }
     } */

    if (is_failed) {
        printf("Error: Could not fill the values of the labels\n");
        exit(EXIT_FAILURE);
    }
}
