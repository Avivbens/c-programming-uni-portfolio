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
 * TODO - we need to check if the label has a prefix, like .extern
 *
 * @returns 1 if the string is a label, 0 otherwise
 */
static int is_label(String str) {
    int i;
    int len = strlen(str);
    if (len > MAX_LABEL_LENGTH + 1) {
        return 0;
    }

    if (!isalpha(str[0]) && (str[0]) != '.') {
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
 * function to determine if a label is external
 */
static int is_label_extern(String str) {
    String label_prefix = get_first_word_from_line(str);
    int is_extern = strcmp(label_prefix, (String)LABEL_EXTERN_PREFIX);

    if (is_extern == 0) {
        return 1;
    }

    return 0;
}

/* function that checks if the token and an opcode have the same name */
int is_command(char *token) {
    int tokenLen = strlen(token);
    if (tokenLen > MAX_COMMAND_LENGTH || tokenLen < MIN_COMMAND_LENGTH)
        return 0;
    /* TODO- Add find_command function*/
    return 1;
}

int is_register(char *token) {
    /* register name starts with 'r' and after there is a number between 0-7 */
    if ((strlen(token) == REGISTER_NAME_LENGTH) && token[0] == 'r' &&
        token[1] >= '0' && token[1] <= '7') {
        return 0;
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
    char token[MAX_LABEL_LENGTH]; /*holds the label */
    String this_word;
    int func_bool;

    String file_path = get_file_name_with_extension(
        file_name, (String)POST_PROCESS_FILE_EXTENSION);
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

    /**
     * Read file line by line
     */
    while (fgets(line, sizeof(line), file)) {
        /**
         * TODO - pass in the entire line, not just the first word
         * Could be external logic
         */
        this_word = get_first_word_from_line(line);
        func_bool = is_label(this_word);

        /**
         * If the word is not a label - Checking whether this is a directive
         * @TODO - Add a function that checks if it is a directive statement
         */
        if (func_bool == 0) {
            continue;
        }

        /**
         * Extract the name of the label itself (without ':') from the first
         * word Check if the word if not already declared in the labels list/as
         * a macro/as an opcode In case the label already exists, we check for
         * external logic If all the tests come out negative, we will add the
         * label to the symbol table
         */
        extractToken(token,
                     this_word); /*Extracting only the characters relevant to
                                    the label name from the word*/
        func_bool = label_check_before_add(
            token); /*Sending the token to the function that checks if the label
                       name already exists as an external/macro/action*/
        if (!func_bool) {
            /**
             * If the token passes all tests successfully, we will set the token
             * as the label name and join the symbol table
             */
            add_label(token, '0');
            func_bool = 0;
            continue;
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
static int fill_symbols_list(String file_name) {
    label_fill(file_name);

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
        label_fill_res = label_fill(file_names[i]);

        if (label_fill_res == NULL) {
            is_failed = EXIT_FAILURE;
        }
    }

    if (is_failed) {
        printf("Error: Could not fill the values of the labels\n");
        exit(EXIT_FAILURE);
    }
}
