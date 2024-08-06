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
 * @param str - The string we want to check
 * @returns 1 if the string is a valid content of a string, 0 otherwise
 */

static int is_valid_string(String str) {
    int i;
    int len = strlen(str);

    for (i = 0; i < len - 1; i++) {
        if (!isalnum(str[i])) {
            return 0;
        }
    }

    return 1;
}

/**
 * @param str - The string we want to check
 * @returns 1 if the string is a valid content of a string, 0 otherwise
 */

static int count_numbers(char *str, int start_index) {
    int count;
    int in_number = 0;
    int comma_found = 0;

    /* Advance the pointer to the start index*/

    str += start_index;

    while (*str) {
        if (isspace((unsigned char)*str)) {
            str++;
            continue;
        }

        if (*str == ',') {
            /*Check if there is no sequence of commas */
            if (in_number == 1) {
                in_number = 0;
                comma_found = 1;
                str++;
                continue;
            } else {
                printf(
                    "Error: str '%s' is invalid string, there is a sequence of "
                    "two characters of type ','\n",
                    str);
                return EXIT_FAILURE;
            }
        }
    }
    if (isdigit((unsigned char)*str) || *str == '+' || *str == '-') {
        if (!in_number) {
            const char *start = str;
            while (isdigit((unsigned char)*str) || *str == '+' || *str == '-') {
                str++;
                /*Invalid character in number*/
                if (*str != '\0' && *str != ',' &&
                    !isspace((unsigned char)*str)) {
                    printf("Error: invalid character inside the number '%s'\n",
                           str);
                    return -1;
                }
                count++;
                in_number = 1;
            }
            comma_found = 0;
        } else {
            /* Invalid character in the string*/
            printf("Error: invalid character inside the string '%s'\n", str);
            return -1;
        }
    }
    /* Check for trailing comma*/
    if (comma_found) {
        printf(
            "Error: str '%s' is invalid string, there is a sequence of two "
            "characters of type ','\n",
            str);
        return -1;
    }
    return count;
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
    int line_number = 0;

    String word;
    int helper;

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
        line_number++;
        label_type = is_label(trim_string(line));

        if (label_type == LABEL_STRING || label_type == LABEL_DATA ||
            label_type == NOT_LABEL_TYPE) {
            word = get_word(line, 1);
            if (strcmp(word, LABEL_EXTERN_PREFIX) == 0) {
                printf(
                    "WARNING: A label defined at the beginning of the "
                    "extern line is invalid\n");
                continue;
            }

            if (strcmp(word, LABEL_ENTRY_PREFIX) == 0) {
                printf(
                    "WARNING: A label defined at the beginning of the "
                    "entry line is invalid\n");
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
                word = get_word(line, 1);
                if (is_command(word) == 0) {
                    printf(
                        "Error: %s Not an opcode, %s label can not be added to "
                        "symbols table\n",
                        word, get_word(line, 0));
                    exit_code = EXIT_FAILURE;
                }
                helper = count_operands(line, 2);
                if (helper == -1) {
                    printf(
                        "Error: Label %s can't be added to symbols table, "
                        "invalid operands\n",
                        word);
                    exit_code = EXIT_FAILURE;
                    continue;
                } else {
                    helper = validate_opcode_operand(word, helper);
                    if (helper == -1) {
                        printf("Error: Opcode %s can not accept %s operands \n",
                               word, helper);
                        exit_code = EXIT_FAILURE;
                        continue;
                    } else {
                        word = replace_substring(strdup(get_word(line, 0)),
                                                 (String) ":", (String) "");
                        add_label(word, NOT_LABEL_TYPE, IC);
                        if (add_label == 1) {
                            printf(
                                "Error: Label %s can't be added to symbols "
                                "table\n",
                                word);
                            exit_code = EXIT_FAILURE;
                            continue;
                        }
                        /*Add the count + 1 for the opcode*/
                        get_instruction_counter(helper + 1);
                    }
                }
                continue;

            case LABEL_EXTERN:
                word = replace_substring(strdup(get_word(line, 1)),
                                         (String) ":", (String) "");
                add_label(word, LABEL_EXTERN, IC);
                if (add_label == 1) {
                    printf("Error: Label %s can't be added to symbols table\n",
                           word);
                    exit_code = EXIT_FAILURE;
                    continue;
                }
                continue;

            case LABEL_ENTRY:
                /* Read the next line */
                if (fgets(line, sizeof(line), file)) {
                    /* Trim the line*/
                    char *trimmed_line = trim_string(line);

                    /* Check if the line is empty*/
                    if (strlen(trimmed_line) == 0) {
                        printf(
                            "Error: The line following the ENTRY label is "
                            "empty.\n");
                        exit_code = EXIT_FAILURE;
                    } else {
                        /* Validate the structure of the next line- same process
                         * as NOT_LABEL_TYPE */
                        word = get_word(line, 1);
                        if (is_command(word) == 0) {
                            printf(
                                "Error: %s Not an opcode, %s label can not be "
                                "added to "
                                "symbols table\n",
                                word, get_word(line, 0));
                            exit_code = EXIT_FAILURE;
                        }
                        helper = count_operands(line, 2);
                        if (helper == -1) {
                            printf(
                                "Error: Label %s can't be added to symbols "
                                "table, invalid operands\n",
                                word);
                            exit_code = EXIT_FAILURE;
                            continue;
                        } else {
                            helper = validate_opcode_operand(word, helper);
                            if (helper == -1) {
                                printf(
                                    "Error: Opcode %s can not accept %s "
                                    "operands \n",
                                    word, helper);
                                exit_code = EXIT_FAILURE;
                                continue;
                            } else {
                                word = replace_substring(
                                    strdup(get_word(line, 0)), (String) ":",
                                    (String) "");
                                add_label(word, NOT_LABEL_TYPE, IC);
                                if (add_label == 1) {
                                    printf(
                                        "Error: Label %s can't be added to "
                                        "symbols table\n",
                                        word);
                                    exit_code = EXIT_FAILURE;
                                    continue;
                                }
                                /*Add the count + 1 for the opcode*/
                                get_instruction_counter(helper + 1);
                            }
                        }

                        continue;

                        case LABEL_DATA:
                            word = get_word(line, 3);
                            helper = count_numbers(line, 3);
                            if (helper == -1) {
                                printf(
                                    "Error: Invalid string %s ,label %s can't "
                                    "be added to "
                                    "symbols table\n",
                                    word, get_word(line, 0));
                                exit_code = EXIT_FAILURE;
                            }
                            word = replace_substring(strdup(get_word(line, 0)),
                                                     (String) ":", (String) "");
                            add_label(word, LABEL_STRING, DC);
                            if (add_label == 1) {
                                printf(
                                    "Error: Label %s can't be added to symbols "
                                    "table\n",
                                    word);
                                exit_code = EXIT_FAILURE;
                                continue;
                            }
                            get_data_counter(helper);
                            continue;

                        case LABEL_STRING:
                            /*Checks if the string that appears after the label
                             * is correct before adding the label*/
                            word = get_word(line, 3);
                            word = remove_quotation(word);
                            is_valid_string(word);
                            if (is_valid_string == 0) {
                                printf(
                                    "Error: Invalid string %s ,label %s can't "
                                    "be added to "
                                    "symbols table\n",
                                    word, get_word(line, 0));
                                exit_code = EXIT_FAILURE;
                                continue;
                            }
                            /*If the string is correct, we will add the label to
                             * the symbol table*/
                            word = replace_substring(strdup(get_word(line, 0)),
                                                     (String) ":", (String) "");
                            add_label(word, LABEL_STRING, DC);
                            if (add_label == 1) {
                                printf(
                                    "Error: Label %s can't be added to symbols "
                                    "table\n",
                                    word);
                                exit_code = EXIT_FAILURE;
                                continue;
                            }
                            /*increment dc according to word length + '0' at the
                             * end */
                            get_data_counter(strlen(get_word(line, 3)) - 1);
                            continue;
                    }
                }

                return exit_code;
        }

        void *handle_labels(String * file_names) {
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

            return NULL;
        }
