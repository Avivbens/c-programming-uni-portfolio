#include "labels-parser.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @param name - the prefix of a label (e.g. .extern)
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
 * @returns 1 if the string is a label, 0 otherwise
 *
 * @note
 * A label is a string that starts with a letter or a dot, followed by a string
 * Cannot start by a digit, and cannot contain any special characters
 */
static LabelType is_label(String line) {
    LabelType label_type;
    int len = strlen(line);
    int word_len;
    int flag;

    String first_word = (String)malloc(len * sizeof(char));
    if (first_word == NULL) {
        printf("Error: Could not allocate memory for %s \n", first_word);
        exit(EXIT_FAILURE);
    }

    first_word = get_word(line, 0);
    word_len = strlen(first_word);

    if (line[len - 1] != ':' || first_word[word_len] != ':') {
        return NOT_LABEL;
    }

    /**
     * Handle special cases for labels - entry, extern, etc
     */
    if (first_word[0] == '.') {
        label_type = get_label_type(first_word);
        return label_type < 2 ? NOT_LABEL : label_type;
    }
    /**
     * Handle basic case for label
     */
    else {
        flag = is_label_name_ok(first_word);
        return flag == 0 ? NOT_LABEL : NOT_LABEL_TYPE;
    }
}

/**
 * @param name - the name of the potential label
 *
 * @returns EXIT_SUCCESS if the label is allowed, EXIT_FAILURE otherwise
 * @throw EXIT_FAILURE if the label is not allowed
 *
 * @note
 * We cannot use operators names, register names, or command names, macro names,
 * etc as labels
 */
static int is_label_name_allowed(String name) {
    if (!is_label_name_ok(name)) {
        return EXIT_FAILURE;
    }

    /* Cannot be a macro name */
    if (has_macro(name)) {
        printf(
            "Error: label '%s' name is not valid- already defined as a macro\n",
            name);
        return EXIT_FAILURE;
    }

    /* Cannot be a native operand */
    if (is_command(name)) {
        printf(
            "Error: label '%s' name is not valid- already defined as a command "
            "name\n",
            name);
        return EXIT_FAILURE;
    }

    /* Cannot be a register */
    if (is_register(name)) {
        printf(
            "Error: label '%s' name is not valid- already defined as a "
            "register name\n",
            name);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
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
    int func_bool;
    int len = strlen(line);

    String first_word = (String)malloc(len * sizeof(char));
    String second_word = (String)malloc(len * sizeof(char));

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
        func_bool = is_label(trim_string(line));

        switch (func_bool) {
            case NOT_LABEL:
                continue;

            case NOT_LABEL_TYPE:
                /** A case where the tested word is an optional name for the
                 * label. In this case we will examine three options: The next
                 * word is a directive type(string or data), we will add it to
                 * the symbol table with the relevant indication. The next word
                 * is a directive type (external or entry)- we will issue a
                 * warning message since this structure is not correct The
                 * following word is not a directive word, we will add the label
                 * to the symbol table without a prompt indication.
                 */
                first_word = get_word(line, 0);
                if (!is_label_name_allowed(first_word)) {
                    continue;
                } else {
                    /*We will check the next word to know if it is a directive
                     * type label */
                    second_word = get_word(line, 1);
                    if (get_label_type(second_word) == LABEL_EXTERN) {
                        printf(
                            "WARNING':A label defined at the beginning of the "
                            "extern line is invalid\n");
                        continue;
                    }
                    if (get_label_type(second_word) == LABEL_ENTRY) {
                        printf(
                            "WARNING':A label defined at the beginning of the "
                            "entry line is invalid\n");
                        continue;
                    }
                    if (get_label_type(second_word) == LABEL_DATA) {
                        add_label(first_word, 0, LABEL_DATA);
                    }
                    if (get_label_type(second_word) == LABEL_STRING) {
                        add_label(first_word, 0, LABEL_STRING);
                    }
                    /*If the first word is a regular label- without a directive
                       type */
                    else if (get_label_type(second_word) == NOT_LABEL_TYPE) {
                        add_label(first_word, 0, NOT_LABEL_TYPE);
                    }
                }

            case LABEL_EXTERN:
                second_word = get_word(line, 1);
                if (get_label_type(second_word) == LABEL_EXTERN) {
                    add_label(first_word, 0, LABEL_EXTERN);
                }
                printf(
                    "WARNING':A label defined at the beginning of the extern "
                    "line is invalid\n");
                continue;

            case LABEL_ENTRY:
                printf(
                    "WARNING':A label defined at the beginning of the entry "
                    "line is invalid\n");
                continue;

            case LABEL_DATA:
                second_word = get_word(line, 1);
                if (!is_label_name_allowed(second_word)) {
                    continue;
                } else {
                    add_label(second_word, 0, LABEL_DATA);
                }

            case LABEL_STRING:
                second_word = get_word(line, 1);
                if (!is_label_name_allowed(second_word)) {
                    continue;
                } else {
                    add_label(second_word, 0, LABEL_STRING);
                }
        }

        /**
         * TODO - add the label based on it's type
         * Apply checks in the `add_label` function
         *
         * @see {@link is_label_name_allowed}
         */
        if (func_bool == NOT_LABEL_TYPE) {
            first_word = get_word(line, 0);
            if (!is_label_name_allowed(first_word)) {
                continue;
            }
            second_word = get_word(line, 1);

            if (get_label_type(second_word) == LABEL_DATA ||
                get_label_type(second_word) == LABEL_STRING) {
                if (!is_label_name_allowed(first_word)) {
                    continue;
                }
            } else if (get_label_type(second_word) == LABEL_ENTRY ||
                       get_label_type(second_word) == LABEL_EXTERN) {
                /* WE CAN ADD A WARNING MESSAGE*/
                continue;
            } else {
                is_label_name_allowed(first_word);
                add_label(first_word, 0, ); /* FIX VALUE!!!!!*/
            }
        }

        else if (func_bool == LABEL_ENTRY) {
            add_label(first_word, 0, LABEL_ENTRY);
        }

        /**
         * Handle the case where the label is already declared
         * @TODO
         */
        exit_code = EXIT_FAILURE;
    }

    return exit_code;
}

/**
 * @deprecated - we can use the {@link label_registration} function to support
 * this logic

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

        update_list(symbolsTable, first_word, instCounter + word_counter);
    }
}
*/

/**
 * @deprecated - we can use the {@link label_registration} function to support
 * this logic
 */
/* static int fill_symbols_list(String file_name) {
    label_fill(file_name);

    instCounter += SYMBOL_START_POINT;
    return FILL_SUCCESS;
} */

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
