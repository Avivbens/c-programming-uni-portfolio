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
    if (strcmp(second_word, LABEL_DATA_PREFIX) == 0) {
        return LABEL_DATA;
    }

    if (strcmp(second_word, LABEL_STRING_PREFIX) == 0) {
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
 * Validate that a string contains only valid characters for a string
 *
 * @param str - The string we want to check
 * @returns 1 if the string is a valid content of a string, 0 otherwise
 */
static int is_valid_string(String str) {
    int i;
    int len = strlen(str);

    if (str[0] != '"' && str[len - 1] != '"') {
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
    if (get_command(name) != NULL) {
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
LabelType is_label(String line) {
    LabelType label_type = get_label_type(line);
    String label_name = NULL;
    String helper = NULL;

    if (label_type == NOT_LABEL) {
        return NOT_LABEL;
    }

    /**
     * In entry and extern labels, the label name is the second word in the line
     */
    if (label_type == LABEL_EXTERN || label_type == LABEL_ENTRY) {
        label_name = get_word(line, 1);

        if (!is_label_name_allowed(label_name)) {
            free(label_name);
            label_name = NULL;

            return LABEL_VIOLATION;
        }

        free(label_name);
        label_name = NULL;

        return label_type;
    }

    /**
     * In data and string labels, the label name is first word in the line,
     * followed by `:`
     */
    label_name = get_word(line, 0);
    if (!ends_with(label_name, (String) ":")) {
        free(label_name);
        label_name = NULL;

        return NOT_LABEL;
    }

    helper = replace_substring(label_name, (String) ":", (String) "");
    free(label_name);
    label_name = NULL;

    if (!is_label_name_allowed(helper)) {
        free(helper);
        helper = NULL;

        return LABEL_VIOLATION;
    }

    free(helper);
    helper = NULL;

    return label_type;
}

/**
 * Update the exit code if needed
 */
static int update_exit_code(int exit_code, int new_code) {
    if (exit_code == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    return new_code;
}

/**
 * Handle the registration of a label that is not a type label
 *
 * @param line - the line to handle
 * @param line_number - the line number
 *
 * @throw In case of an error, it would return EXIT_FAILURE
 * @returns EXIT_SUCCESS if the entry label was registered successfully
 */
static int handle_no_type_label_reg(String line, int line_number) {
    String opcode = get_word(line, 1);
    String rest = substring_words(line, 2);

    /**
     * -----------------------
     * Extract the label name
     * -----------------------
     */
    String label_raw = get_word(line, 0);
    String label = replace_substring(label_raw, (String) ":", (String) "");

    String helper_str;
    int helper_int;

    String *operands = NULL;
    int operands_amount = 0;
    OpcodeCheck res;

    free(label_raw);
    label_raw = NULL;

    /**
     * ---------------------
     * Clean the rest string
     * ---------------------
     */
    helper_str = trim_string(rest);
    free(rest);
    rest = NULL;

    rest = helper_str;
    helper_str = NULL;

    /**
     * ---------------------
     * Extract the operands
     * ---------------------
     */
    if (rest != NULL) {
        operands = split_string(rest, (String) ",");
        operands_amount = get_string_array_length(operands, sizeof(String));
    } else {
        operands_amount = 0;
    }

    res = validate_opcode_operand(opcode, operands_amount);

    if (res == NOT_EXISTS) {
        printf(
            "line: %d, Error(handle_no_type_label_reg): Opcode '%s' not "
            "exists! \n",
            line_number, opcode);

        free(opcode);
        opcode = NULL;

        free(label);
        label = NULL;

        return EXIT_FAILURE;
    }

    if (res == INVALID_OPERANDS) {
        printf(
            "line: %d, Error(handle_no_type_label_reg): Opcode '%s' can not "
            "accept %d operands \n",
            line_number, opcode, operands_amount);

        free(opcode);
        opcode = NULL;

        free(label);
        label = NULL;

        return EXIT_FAILURE;
    }

    free(opcode);
    opcode = NULL;

    /**
     * ---------------------
     * Add the label
     * ---------------------
     */
    helper_int = add_label(label, NOT_LABEL_TYPE,
                           get_instruction_counter(operands_amount + 1));
    if (helper_int != EXIT_SUCCESS) {
        printf(
            "Error(handle_no_type_label_reg): label '%s' can not be added to "
            "symbols table\n",
            label);

        free(label);
        label = NULL;
        return EXIT_FAILURE;
    }

    free(label);
    label = NULL;

    return EXIT_SUCCESS;
}

/**
 * Handle the increment of the instruction & data counter
 *
 * @param line - the line to handle
 * @param line_number - the line number
 */
static int handle_not_label_reg(String line, int line_number) {
    String trimmed_line = trim_string(line);
    String first_word;
    String rest;

    String *operands;
    int operands_amount;

    OpcodeBinary *opcode;
    int to_increment_by;

    int exit_code = EXIT_FAILURE;

    first_word = get_word(trimmed_line, 0);
    opcode = get_command(first_word);

    if (opcode != NULL) {
        /**
         * TODO - validate operands number
         */

        to_increment_by = opcode->operands + 1;
        get_instruction_counter(to_increment_by);
        exit_code = EXIT_SUCCESS;
    }

    if (strcmp(first_word, (String)LABEL_DATA_PREFIX) == 0) {
        /**
         * TODO - validate operands number
         */

        rest = substring_words(trimmed_line, 1);
        operands = split_string(rest, (String) ",");
        operands_amount = get_string_array_length(operands, sizeof(String));

        get_data_counter(operands_amount);
        exit_code = EXIT_SUCCESS;
    }

    if (strcmp(first_word, (String)LABEL_STRING_PREFIX) == 0) {
        /**
         * TODO - validate operands number
         */

        rest = substring_words(trimmed_line, 1);
        operands_amount = strlen(rest) - 2 + 1;

        get_data_counter(operands_amount);
        exit_code = EXIT_SUCCESS;
    }

    if (exit_code == EXIT_FAILURE) {
        printf("line: %d, Error: '%s' is not a valid command\n", line_number,
               first_word);
    }

    return exit_code;
}

/**
 * Handle the registration of a data label
 *
 * @param line - the line to handle
 * @param line_number - the line number
 *
 * @throw In case of an error, it would return EXIT_FAILURE
 * @returns EXIT_SUCCESS if the entry label was registered successfully
 */
static int handle_data_label_reg(String line, int line_number) {
    String rest = substring_words(line, 2);

    /**
     * -----------------------
     * Extract the label name
     * -----------------------
     */
    String label_raw = get_word(line, 0);
    String label = replace_substring(label_raw, (String) ":", (String) "");

    String helper_str;
    int helper_int;

    int i;

    String *numbers = NULL;
    int numbers_amount = 0;

    free(label_raw);
    label_raw = NULL;

    /**
     * ---------------------
     * Clean the rest string
     * ---------------------
     */
    helper_str = trim_string(rest);
    free(rest);
    rest = NULL;

    rest = helper_str;
    helper_str = NULL;

    /**
     * ---------------------
     * Extract the numbers
     * ---------------------
     */
    numbers = split_string(rest, (String) ",");
    numbers_amount = get_string_array_length(numbers, sizeof(String));

    for (i = 0; i < numbers_amount; i++) {
        helper_str = trim_string(numbers[i]);
        if (!is_number(helper_str)) {
            printf("line: %d, Error: '%s' is not a valid number\n", line_number,
                   numbers[i]);

            free(label);
            label = NULL;

            free(helper_str);
            helper_str = NULL;

            return EXIT_FAILURE;
        }

        free(helper_str);
        helper_str = NULL;
    }

    /**
     * ---------------------
     * Add the label
     * ---------------------
     */
    helper_int = add_label(label, LABEL_DATA, get_data_counter(numbers_amount));
    if (helper_int != 0) {
        printf(
            "line: %d, Error: label '%s' can not be added to symbols table\n",
            line_number, label);

        free(label);
        label = NULL;

        return EXIT_FAILURE;
    }

    free(label);
    label = NULL;

    return EXIT_SUCCESS;
}

/**
 * Handle the registration of an entry label
 *
 * @param line - the line to handle
 * @param line_number - the line number
 *
 * @throw In case of an error, it would return EXIT_FAILURE
 * @returns EXIT_SUCCESS if the entry label was registered successfully
 */
static int handle_entry_label_reg(String line, int line_number) {
    String opcode = get_word(line, 1);
    String rest = substring_words(line, 2);

    /**
     * -----------------------
     * Extract the label name
     * -----------------------
     */
    String label_raw = get_word(line, 0);
    String label = replace_substring(label_raw, (String) ":", (String) "");

    String helper_str;
    int helper_int;

    String *operands;
    int operands_amount = 0;
    OpcodeCheck res;

    free(label_raw);
    label_raw = NULL;

    /**
     * ---------------------
     * Clean the rest string
     * ---------------------
     */
    helper_str = trim_string(rest);
    free(rest);
    rest = helper_str;

    /**
     * ---------------------
     * Extract the operands
     * ---------------------
     */
    operands = split_string(rest, (String) ",");
    operands_amount = get_string_array_length(operands, sizeof(String));

    free_string_array_recursively(operands, operands_amount);
    operands = NULL;

    free(rest);
    rest = NULL;

    /**
     * ---------------------
     * Case: Opcode not exists
     * ---------------------
     */
    res = validate_opcode_operand(opcode, operands_amount);
    if (res == NOT_EXISTS) {
        printf(
            "line: %d, Error(handle_entry_label_reg): Opcode '%s' not exists! "
            "\n",
            line_number, opcode);

        free(label);
        label = NULL;

        free(opcode);
        opcode = NULL;

        return EXIT_FAILURE;
    }

    /**
     * ---------------------
     * Case: Invalid operands
     * ---------------------
     */
    if (res == INVALID_OPERANDS) {
        printf(
            "line: %d, Error(handle_entry_label_reg): Opcode '%s' can not "
            "accept %d operands \n",
            line_number, opcode, operands_amount);

        free(label);
        label = NULL;

        free(opcode);
        opcode = NULL;

        return EXIT_FAILURE;
    }

    free(opcode);
    opcode = NULL;

    /**
     * ---------------------
     * Case: global label error
     * ---------------------
     */
    helper_int = add_label(label, LABEL_ENTRY,
                           get_instruction_counter(operands_amount + 1));

    if (helper_int != 0) {
        printf(
            "Error(handle_entry_label_reg): label '%s' can not be added to "
            "symbols table\n",
            label);

        free(label);
        label = NULL;

        return EXIT_FAILURE;
    }

    free(label);
    label = NULL;

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
static int label_registration(String file_path) {
    FILE *file;
    int exit_code = EXIT_SUCCESS;
    char line[MAX_LINE_LENGTH];
    int line_number = 0;
    LabelType label_type;

    String word;
    String helper;
    int res;
    int flag;

    file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Error: Could not open file '%s'\n", file_path);
        return EXIT_FAILURE;
    }

    /**
     * Read file line by line
     */
    while (fgets(line, sizeof(line), file)) {
        line_number++;
        if (is_comment(line)) {
            continue;
        }

        helper = trim_string(line);
        label_type = is_label(helper);

        free(helper);
        helper = NULL;

        if (label_type == LABEL_STRING || label_type == LABEL_DATA ||
            label_type == NOT_LABEL_TYPE) {
            flag = 0;
            word = get_word(line, 1);

            if (strcmp(word, LABEL_EXTERN_PREFIX) == 0) {
                printf(
                    "line: %d, WARNING: A label defined at the beginning of "
                    "the extern line is invalid\n ",
                    line_number);
                flag = 1;
            }

            if (strcmp(word, LABEL_ENTRY_PREFIX) == 0) {
                printf(
                    "line: %d, WARNING: A label defined at the beginning of "
                    "the entry line is invalid\n",
                    line_number);
                flag = 1;
            }

            free(word);
            word = NULL;

            if (flag) {
                continue;
            }
        }

        switch ((int)label_type) {
            case LABEL_VIOLATION:
                printf("line: %d, Error: violation with labels system!\n",
                       line_number);
                exit_code = EXIT_FAILURE;
                continue;

            case NOT_LABEL:
                res = handle_not_label_reg(line, line_number);
                exit_code = update_exit_code(exit_code, res);
                continue;

            case NOT_LABEL_TYPE:
                res = handle_no_type_label_reg(line, line_number);
                exit_code = update_exit_code(exit_code, res);
                continue;

            case LABEL_EXTERN:
                helper = get_word(line, 1);
                res =
                    add_label(helper, LABEL_EXTERN, get_instruction_counter(0));
                exit_code = update_exit_code(exit_code, res);

                free(helper);
                helper = NULL;
                continue;

            case LABEL_ENTRY:
                /*skip to the next line*/
                res = handle_entry_label_reg(line, line_number);
                exit_code = update_exit_code(exit_code, res);
                continue;

            case LABEL_DATA:
                res = handle_data_label_reg(line, 2);
                exit_code = update_exit_code(exit_code, res);
                continue;

            case LABEL_STRING:
                helper = substring_words(line, 2);
                res = is_valid_string(helper);

                free(helper);
                helper = NULL;

                if (res != 1) {
                    printf(
                        "Error: invalid String, can not add label to symbols "
                        "table\n ");
                    continue;
                }

                /* Get label name */
                helper = get_word(line, 0);
                word = replace_substring(helper, (String) ":", (String) "");

                free(helper);
                helper = NULL;

                /* Get string value */
                helper = get_word(line, 2);
                res = add_label(word, LABEL_STRING,
                                get_data_counter(strlen(helper) - 2 + 1));

                free(helper);
                helper = NULL;

                exit_code = update_exit_code(exit_code, res);
                continue;
        }
    }

    return exit_code;
}

/**
 * Register and create all labels for processed files
 *
 * @param file_paths the post processed files paths, to the dist directory
 *
 * @throw In case of an error, it would exit the program with EXIT_FAILURE
 */
void *handle_labels(String *file_paths) {
    int i;
    int is_failed = EXIT_SUCCESS;
    int label_reg_res = EXIT_SUCCESS;

    /**
     * Register labels
     */
    for (i = 0; file_paths[i] != NULL; i++) {
        label_reg_res = label_registration(file_paths[i]);

        if (label_reg_res == EXIT_FAILURE) {
            printf("Error: Could not register labels in file: '%s' \n",
                   file_paths[i]);

            is_failed = EXIT_FAILURE;
        }
    }

    if (is_failed) {
        printf("Error: Could not register labels\n");
        exit(EXIT_FAILURE);
    }

    return NULL;
}
