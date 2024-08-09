#include "labels-parser.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * The Instruction Counter begins at 100 and increases by the number of memory
 * cells each instruction uses.
 *
 * This indicates the next available memory cell.
 */
static int get_instruction_counter(int increment) {
    static int instruction_counter = SYMBOL_START_POINT;
    int res = instruction_counter;

    instruction_counter += increment;
    return res;
}

/**
 * The Data Counter serves a similar purpose as the Instruction Counter but is
 * specifically for data.
 *
 * For safety reasons, they are kept separate.
 *
 * TODO - keep them separate
 */
static int get_data_counter(int increment) {
    static int data_counter = SYMBOL_START_POINT + 100;
    int res = data_counter;

    data_counter += increment;
    return res;
}

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

static int update_exit_code(int exit_code, int new_code) {
    if (exit_code == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    return new_code;
}

static int handle_no_type_label_reg(String line, int line_number) {
    String opcode = get_word(line, 1);
    String rest = substring_words(line, 2);
    String label =
        replace_substring(strdup(get_word(line, 0)), (String) ":", (String) "");
    int helper;

    String *operands;
    int operands_amount = 0;
    OpcodeCheck res;

    rest = trim_string(rest);

    operands = split_string(rest, (String) ",");
    operands_amount = get_array_length(operands, sizeof(String));

    res = validate_opcode_operand(opcode, operands_amount);

    if (res == NOT_EXISTS) {
        printf("line: %d, Error: Opcode %s not exists! \n", line_number,
               opcode);

        return EXIT_FAILURE;
    }

    if (res == INVALID_OPERANDS) {
        printf("line: %d, Error: Opcode %s can not accept %d operands \n",
               line_number, opcode, operands_amount);

        return EXIT_FAILURE;
    }

    helper = add_label(label, NOT_LABEL_TYPE,
                       get_instruction_counter(operands_amount + 1));
    if (helper != 0) {
        printf("Error: label %s can not be added to symbols table\n", label);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

static int handle_data_label_reg(String line, int line_number) {
    String rest = substring_words(line, 2);
    String label =
        replace_substring(strdup(get_word(line, 0)), (String) ":", (String) "");
    int helper;

    String *numbers;
    int numbers_amount = 0;

    rest = trim_string(rest);

    numbers = split_string(rest, (String) ",");
    numbers_amount = get_array_length(numbers, sizeof(String));

    helper = add_label(label, LABEL_DATA, get_data_counter(numbers_amount));
    if (helper != 0) {
        printf("Error: label %s can not be added to symbols table\n", label);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

static int handle_entry_label_reg(String line, int line_number) {
    String opcode = get_word(line, 1);
    String rest = substring_words(line, 2);
    String label =
        replace_substring(strdup(get_word(line, 0)), (String) ":", (String) "");
    int helper;

    String *operands;
    int operands_amount = 0;
    OpcodeCheck res;

    rest = trim_string(rest);

    operands = split_string(rest, (String) ",");
    operands_amount = get_array_length(operands, sizeof(String));

    res = validate_opcode_operand(opcode, operands_amount);

    if (res == NOT_EXISTS) {
        printf("line: %d, Error: Opcode %s not exists! \n", line_number,
               opcode);

        return EXIT_FAILURE;
    }

    if (res == INVALID_OPERANDS) {
        printf("line: %d, Error: Opcode %s can not accept %d operands \n",
               line_number, opcode, operands_amount);

        return EXIT_FAILURE;
    }

    helper = add_label(label, LABEL_ENTRY,
                       get_instruction_counter(operands_amount + 1));
    if (helper != 0) {
        printf("Error: label %s can not be added to symbols table\n", label);
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
    int line_number = 0;
    LabelType label_type;

    String word;
    int res;

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
        line_number++;
        label_type = is_label(trim_string(line));

        if (label_type == LABEL_STRING || label_type == LABEL_DATA ||
            label_type == NOT_LABEL_TYPE) {
            word = get_word(line, 1);
            if (strcmp(word, LABEL_EXTERN_PREFIX) == 0) {
                printf(
                    "line: %d, WARNING: A label defined at the beginning of "
                    "the extern line is invalid\n ",
                    line_number);
                continue;
            }

            if (strcmp(word, LABEL_ENTRY_PREFIX) == 0) {
                printf(
                    "line: %d, WARNING: A label defined at the beginning of "
                    "the entry line is invalid\n",
                    line_number);
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
                continue;

            case NOT_LABEL_TYPE:
                res = handle_no_type_label_reg(line, line_number);
                exit_code = update_exit_code(exit_code, res);
                continue;

            case LABEL_EXTERN:
                res = add_label(get_word(line, 1), LABEL_EXTERN,
                                get_instruction_counter(0));
                exit_code = update_exit_code(exit_code, res);
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
                res = is_valid_string(substring_words(line, 2));
                if (res != 1) {
                    printf(
                        "Error: invalid String, can not add label to symbols "
                        "table\n ");
                    continue;
                }
                word = replace_substring(strdup(get_word(line, 0)),
                                         (String) ":", (String) "");
                res = add_label(word, LABEL_STRING,
                                get_data_counter(strlen(word) - 2));
                exit_code = update_exit_code(exit_code, res);
                continue;
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
            printf("Error: Could not register labels in file: %s \n",
                   file_names[i]);

            is_failed = EXIT_FAILURE;
        }
    }

    if (is_failed) {
        printf("Error: Could not register labels\n");
        exit(EXIT_FAILURE);
    }

    return NULL;
}
