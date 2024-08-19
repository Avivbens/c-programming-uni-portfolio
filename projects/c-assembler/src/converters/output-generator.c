#include "output-generator.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * The Instruction Counter indicate which output line we're currently on
 * Starting from 100
 *
 * @attention - free this memory after use
 */
static String get_output_line_counter(int increment) {
    static int instruction_counter = IC_OUTPUT_START_POINT;
    int res = instruction_counter;
    String str_res;
    String helper = NULL;

    instruction_counter += increment;

    helper = cast_decimal_to_string(res);
    str_res = pad_left(helper, 4, '0');

    free(helper);
    helper = NULL;

    return str_res;
}

/**
 * Extract the opcode from a line
 *
 * @attention - free this memory after use
 *
 * @param line the line to extract the opcode from
 *
 * @returns the opcode name
 *
 * @note if the line is a label, the opcode is the second word
 */
static String extract_opcode(String line, LabelType label_type) {
    String opcode = NULL;
    String helper = NULL;

    int fixed_index = label_type == NOT_LABEL ? 0 : 1;

    helper = get_word(line, fixed_index);
    opcode = trim_string(helper);

    free(helper);
    helper = NULL;

    return opcode;
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
 * Generate the binary code for the opcode
 *
 * @param opcode the opcode to generate the binary code for
 *
 * @returns the binary code for the opcode
 */
static String get_binary_by_address_mode(AddressMode address_mode) {
    switch (address_mode) {
        case IMMEDIATE_ADDRESS_MODE:
            return (String) "0001";
        case DIRECT_ADDRESS_MODE:
            return (String) "0010";
        case INDIRECT_ACCUMULATED_ADDRESS_MODE:
            return (String) "0100";
        case DIRECT_ACCUMULATED_ADDRESS_MODE:
            return (String) "1000";
        default:
            return NULL;
    }
}

/**
 * Handle the opcode binary generation
 */
static int handle_opcode(int line_number, String line_res, String opcode) {
    int operand_count = get_operands_number_per_opcode(opcode);
    String opcode_binary = generate_opcode_binary(opcode);
    if (opcode_binary == NULL || operand_count == -1) {
        printf("line: %d, Error: Invalid opcode - '%s'\n", line_number, opcode);
        opcode = NULL;

        return EXIT_FAILURE;
    }

    strcat(line_res, opcode_binary);
    return EXIT_SUCCESS;
}

/**
 * Generate the binary code for all operands
 *
 * @param line_number the line number
 * @param line_res the result line
 * @param line the line to generate the binary code for
 * @param opcode the opcode
 * @param label_type the label type
 *
 * @throw EXIT_FAILURE if the operand is invalid
 * @returns EXIT_SUCCESS if the operand is valid
 *
 * @note
 * This function handle just the first line (the opcode output line)
 */
static int handle_opcode_operands(int line_number, String line_res, String line,
                                  String opcode, LabelType label_type) {
    int operand_count = get_operands_number_per_opcode(opcode);
    String operand;
    AddressMode address_mode;
    String helper;

    int i;

    /**
     * Handle all the operands
     * We start with the last operand (the source operand)
     *
     * There are 3 cases - no operands, one operand (only source), two
     * operands
     */
    for (i = 2; i >= 1; i--) {
        if (i > operand_count) {
            /**
             * Insert empty data into the results, in case there is no
             * operand
             */

            strcat(line_res, "0000");
            continue;
        }

        /**
         * Due to the fact we're going from the last operand to the first,
         * we need to extract the operand in the following way
         */
        operand = extract_operand(line, label_type, 2 - i);
        address_mode = get_address_mode(operand);

        if (address_mode == ERROR) {
            printf(
                "line: %d, Error(handle_opcode_operands): Invalid operand - "
                "'%s'\n",
                line_number, operand);

            free(operand);
            operand = NULL;

            return EXIT_FAILURE;
        }

        helper = get_binary_by_address_mode(address_mode);
        if (helper == NULL) {
            printf("line: %d, Error: Invalid address mode - '%d'\n",
                   line_number, address_mode);

            free(operand);
            operand = NULL;

            return EXIT_FAILURE;
        }

        strcat(line_res, helper);

        free(operand);
        operand = NULL;
    }

    return EXIT_SUCCESS;
}

/**
 * Handle operand that is a number
 *
 * @param line_number the line number
 * @param operand the operand to handle
 *
 * @returns NULL if the operand is valid, otherwise the new line in binary
 */
static String handle_number_operand(int line_number, String operand) {
    String helper1;
    String helper2;
    String binary;

    helper1 = replace_substring(operand, (String) "#", (String) "");
    helper2 = trim_string(helper1);
    binary = cast_decimal_to_binary(helper2);

    free(helper1);
    free(helper2);
    helper1 = NULL;
    helper2 = NULL;

    if (binary == NULL) {
        printf("line: %d, Error: Invalid number - '%s'\n", line_number,
               operand);

        return NULL;
    }

    /* Insert ARE for number - 100 */
    strcat(binary, "100");
    return binary;
}

/**
 * Handle operand that is a label
 *
 * @param line_number the line number
 * @param operand the operand to handle
 *
 * @returns NULL if the operand is valid, otherwise the new line in binary
 */
static String handle_label_operand(int line_number, String operand,
                                   LabelType label_type,
                                   String current_output_line_number) {
    Label *label = NULL;
    String binary;
    String helper1;
    int memory_address;
    int label_ic_to_add;

    helper1 = trim_string(operand);
    label = get_label(helper1);

    free(helper1);
    helper1 = NULL;

    if (label == NULL) {
        printf("line: %d, Error: Invalid label - '%s'\n", line_number, operand);
        return NULL;
    }

    /**
     * Extern labels should not has any memory address for operands output
     */
    if (label->has_extern) {
        memory_address = 0;
    } else {
        if (label_type == LABEL_STRING || label_type == LABEL_DATA) {
            label_ic_to_add = get_data_counter(0);
        } else {
            label_ic_to_add = get_instruction_counter(0);
        }

        memory_address =
            label->memory_address + SYMBOL_START_POINT + label_ic_to_add;
    }

    helper1 = cast_decimal_to_string(memory_address);
    binary = cast_decimal_to_binary(helper1);

    if (label->has_extern) {
        add_output_label(label->name, current_output_line_number,
                         OUTPUT_LABEL_EXTERN);
    }

    if (label->has_entry) {
        add_output_label(label->name, helper1, OUTPUT_LABEL_ENTRY);
    }

    free(helper1);
    helper1 = NULL;

    if (label->has_extern) {
        /* For extern - E in the ARE */
        strcat(binary, "001");
    } else {
        /* For the rest - R in the ARE */
        strcat(binary, "010");
    }

    return binary;
}

/**
 * Handle operand that is a pointer to register
 *
 * @param line_number the line number
 * @param operand the operand to handle
 * @param is_dest if the operand is a destination
 *
 * @returns NULL if the operand is valid, otherwise the new line in binary
 */
static String handle_register_operand(int line_number, String operand,
                                      int is_dest) {
    String helper1;
    String helper2;
    String binary;

    helper1 = replace_substring(operand, (String) "*", (String) "");
    helper2 = trim_string(helper1);
    binary = cast_decimal_to_binary(helper2 + 1);

    free(helper1);
    free(helper2);
    helper1 = NULL;
    helper2 = NULL;

    if (binary == NULL) {
        printf("line: %d, Error: Invalid register - '%s'\n", line_number,
               operand);

        return NULL;
    }

    /* Insert empty dest */
    if (is_dest != 1) {
        strcat(binary, "000");
    }

    /* Insert ARE for register - 100 */
    strcat(binary, "100");
    return binary;
}

/**
 * Handle two registers operands
 *
 * @param line the line to handle
 * @param label_type the label type
 *
 * @returns the binary code for the two registers
 */
static String handle_two_registers_operands(String line, LabelType label_type) {
    String operand;
    String helper1;
    String helper2;
    String helper3;
    String binary = (String)malloc(MAX_LINE_OUTPUT * sizeof(char));

    int i;

    /**
     * Insert all registers numbers into the binary
     */
    for (i = 0; i < 2; i++) {
        operand = extract_operand(line, label_type, i);

        helper1 = replace_substring(operand, (String) "*", (String) "");
        helper2 = trim_string(helper1);
        helper3 = cast_decimal_to_binary(helper2 + 1);

        strcat(binary, helper3);

        free(helper1);
        helper1 = NULL;

        free(helper2);
        helper2 = NULL;

        free(helper3);
        helper3 = NULL;
    }

    free(operand);
    operand = NULL;

    /* Insert ARE */
    strcat(binary, "100");
    return binary;
}

/**
 * Generate the binary code for all operands
 *
 * @param line_number the line number
 * @param line_res the result line
 * @param line the line to generate the binary code for
 * @param opcode the opcode
 * @param label_type the label type
 *
 * @returns The new line (after casting to octal & insert the data counter)
 * In case of an error, NULL is returned
 *
 * @note
 * This function handles the additional lines (0-2 additional)
 */
static String handle_operands_output(int line_number, String line,
                                     String opcode, LabelType label_type) {
    int operand_count = get_operands_number_per_opcode(opcode);
    AddressMode address_mode;
    String operand = NULL;
    int exit_code = EXIT_SUCCESS;

    String rest_line_res = NULL;
    String results;
    String helper1 = NULL;
    String helper2 = NULL;

    int i;
    int all_registers =
        is_all_operands_are_registers(line, label_type, operand_count);
    String current_output_line_number;

    results = (String)malloc(MAX_LINE_OUTPUT * sizeof(char));

    if (all_registers) {
        rest_line_res = handle_two_registers_operands(line, label_type);
        if (rest_line_res == NULL) {
            return NULL;
        }

        strcat(results, get_output_line_counter(1));
        strcat(results, " ");

        helper1 = cast_binary_to_octal(rest_line_res);
        helper2 = pad_left(helper1, 5, '0');
        strcat(results, helper2);

        free(rest_line_res);
        rest_line_res = NULL;

        free(helper1);
        helper1 = NULL;

        free(helper2);
        helper2 = NULL;

        strcat(results, "\n");
        return results;
    }

    for (i = 0; i < operand_count; i++) {
        current_output_line_number = get_output_line_counter(1);
        strcat(results, current_output_line_number);
        strcat(results, " ");

        operand = extract_operand(line, label_type, i);
        address_mode = get_address_mode(operand);
        switch (address_mode) {
            case ERROR:
                printf(
                    "line: %d, Error(handle_operands_output): Invalid operand "
                    "- '%s'\n",
                    line_number, operand);
                exit_code = EXIT_FAILURE;
                break;

                /* Number */
            case IMMEDIATE_ADDRESS_MODE:
                /**
                 * Cast the number to binary
                 */
                rest_line_res = handle_number_operand(line_number, operand);
                if (rest_line_res == NULL) {
                    exit_code = EXIT_FAILURE;
                    break;
                }

                helper1 = cast_binary_to_octal(rest_line_res);
                helper2 = pad_left(helper1, 5, '0');
                strcat(results, helper2);
                break;

                /* Label */
            case DIRECT_ADDRESS_MODE:
                /**
                 * Cast the number to binary
                 */
                rest_line_res =
                    handle_label_operand(line_number, operand, label_type,
                                         current_output_line_number);
                if (rest_line_res == NULL) {
                    exit_code = EXIT_FAILURE;
                    break;
                }

                helper1 = cast_binary_to_octal(rest_line_res);
                helper2 = pad_left(helper1, 5, '0');
                strcat(results, helper2);
                break;

                /* register */
            case DIRECT_ACCUMULATED_ADDRESS_MODE:
            case INDIRECT_ACCUMULATED_ADDRESS_MODE:
                /**
                 * Cast the number to binary
                 *
                 * The register is a destination if it's the second operand, or
                 * there is only 1 operand
                 */
                rest_line_res = handle_register_operand(
                    line_number, operand, operand_count == 1 || i == 1);
                if (rest_line_res == NULL) {
                    exit_code = EXIT_FAILURE;
                    break;
                }

                helper1 = cast_binary_to_octal(rest_line_res);
                helper2 = pad_left(helper1, 5, '0');
                strcat(results, helper2);
                break;
        }

        free(helper1);
        helper1 = NULL;

        free(helper2);
        helper2 = NULL;

        free(rest_line_res);
        rest_line_res = NULL;

        strcat(results, "\n");
    }

    free(operand);
    operand = NULL;

    if (exit_code == EXIT_FAILURE) {
        return NULL;
    }

    return results;
}

/**
 * Handle data line data with / without label
 *
 * @param line_number the line number
 * @param line the line to handle
 * @param line_label_type the label type
 * @param line_res the result line
 *
 * @returns NULL if the line is valid, otherwise the new line (after casting to
 * octal & insert the data counter)
 */
static String handle_data_line(int line_number, String line,
                               LabelType line_label_type) {
    String numbers_string;
    String *numbers;
    int numbers_amount;
    int exit_code = EXIT_SUCCESS;
    String output = NULL;
    int i;

    int extract_from = line_label_type == NOT_LABEL ? 1 : 2;
    String helper1;
    String helper2;

    numbers_string = substring_words(line, extract_from);
    numbers = split_string(numbers_string, (String) ",");
    numbers_amount = get_string_array_length(numbers);

    output = (String)malloc(MAX_LINE_OUTPUT * numbers_amount * sizeof(char));
    if (output == NULL) {
        printf(
            "line: %d, Error(handle_data_line): Could not allocate memory for "
            "binary\n",
            line_number);
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < numbers_amount; i++) {
        helper1 = trim_string(numbers[i]);
        if (!is_number(helper1)) {
            printf("line: %d, Error: '%s' is not a valid number\n", line_number,
                   helper1);

            free(helper1);
            helper1 = NULL;

            exit_code = EXIT_FAILURE;
            continue;
        }

        helper2 = cast_decimal_to_binary(helper1);
        free(helper1);
        helper1 = NULL;

        helper1 = cast_binary_to_octal(helper2);
        free(helper2);

        helper2 = pad_left(helper1, 5, '0');

        strcat(output, get_output_line_counter(1));
        strcat(output, " ");
        strcat(output, helper2);

        if (i + 1 != numbers_amount) {
            strcat(output, "\n");
        }

        free(helper1);
        helper1 = NULL;

        free(helper2);
        helper2 = NULL;
    }

    free(numbers_string);
    numbers_string = NULL;

    if (exit_code == EXIT_FAILURE) {
        return NULL;
    }

    return output;
}

/**
 * Handle string line data with / without label
 *
 * @param line_number the line number
 * @param line the line to handle
 * @param line_label_type the label type
 *
 * @returns NULL if the line is valid, otherwise the new line (after casting to
 * octal & insert the data counter)
 */
static String handle_string_line(int line_number, String line,
                                 LabelType line_label_type) {
    String raw_chars_string;
    String chars_string;
    int chars_amount;
    String output = NULL;
    int i;

    int extract_from = line_label_type == NOT_LABEL ? 1 : 2;
    String helper1;
    String helper2;

    raw_chars_string = substring_words(line, extract_from);
    chars_string = trim_string(raw_chars_string);
    chars_amount = strlen(chars_string) - 2;

    free(raw_chars_string);
    raw_chars_string = NULL;

    output =
        (String)malloc(MAX_LINE_OUTPUT * (chars_amount + 5) * sizeof(char));
    if (output == NULL) {
        printf(
            "line: %d, Error(handle_string_line): Could not allocate memory "
            "for binary\n",
            line_number);
        exit(EXIT_FAILURE);
    }

    for (i = 1; chars_string[i + 1] != '\0'; i++) {
        helper1 = cast_decimal_to_string(chars_string[i]);
        helper2 = cast_decimal_to_binary(helper1);

        free(helper1);

        helper1 = cast_binary_to_octal(helper2);
        free(helper2);

        helper2 = pad_left(helper1, 5, '0');

        strcat(output, get_output_line_counter(1));
        strcat(output, " ");
        strcat(output, helper2);

        if (chars_string[i + 1] != '\0') {
            strcat(output, "\n");
        }

        free(helper1);
        helper1 = NULL;

        free(helper2);
        helper2 = NULL;
    }

    strcat(output, get_output_line_counter(1));
    strcat(output, " ");
    strcat(output, "00000");

    free(chars_string);
    chars_string = NULL;

    return output;
}

/**
 * Generate the output for a file
 *
 * @param file_path the path for the file
 *
 * @returns The new file content, or NULL if the file could not be created
 */
static String generate_file_output(String file_path) {
    FILE *file;
    int exit_code = EXIT_SUCCESS;
    int updated_exit_code;
    char line[MAX_LINE_LENGTH];
    int line_number = 0;

    String opcode = NULL;
    LabelType line_label_type;

    String file_res = (String)malloc(MAX_LINE_OUTPUT *
                                     MAX_LINES_FOR_OUTPUT_LINE * sizeof(char));
    String line_res = NULL;
    String helper = NULL;

    if (file_res == NULL) {
        printf("Error: Could not allocate memory for file_res\n");
        exit(EXIT_FAILURE);
    }

    file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Error: Could not open file '%s'\n", file_path);
        return NULL;
    }

    /**
     * Read file line by line
     */
    while (fgets(line, sizeof(line), file)) {
        line_number++;

        if (is_comment(line)) {
            continue;
        }

        line_label_type = is_label(line);

        line_res = (String)malloc(MAX_LINE_OUTPUT * sizeof(char));
        file_res = (String)realloc(file_res, MAX_LINE_OUTPUT * line_number *
                                                 MAX_LINES_FOR_OUTPUT_LINE *
                                                 sizeof(char));
        if (line_res == NULL | file_res == NULL) {
            printf("Error: Could not allocate memory for line\n");
            exit(EXIT_FAILURE);
        }

        /**
         * No need to generate output for entry / extern labels
         */
        if (line_label_type == LABEL_ENTRY || line_label_type == LABEL_EXTERN) {
            continue;
        }

        /**
         * ---------------------
         * Handle data / string lines
         * ---------------------
         */
        helper = trim_string(line);
        if (line_label_type == LABEL_DATA ||
            starts_with(helper, (String)LABEL_DATA_PREFIX)) {
            free(helper);
            helper = NULL;

            helper = handle_data_line(line_number, line, line_label_type);
            if (helper == NULL) {
                free(line_res);
                line_res = NULL;

                exit_code = EXIT_FAILURE;
                continue;
            }

            strcat(file_res, helper);
            strcat(file_res, "\n");

            free(line_res);
            line_res = NULL;

            free(helper);
            helper = NULL;
            continue;
        }

        if (line_label_type == LABEL_STRING ||
            starts_with(helper, (String)LABEL_STRING_PREFIX)) {
            helper = handle_string_line(line_number, line, line_label_type);
            if (helper == NULL) {
                free(line_res);
                line_res = NULL;

                exit_code = EXIT_FAILURE;
                continue;
            }

            strcat(file_res, helper);
            strcat(file_res, "\n");

            free(helper);
            helper = NULL;
            continue;
        }

        free(helper);
        helper = NULL;

        opcode = extract_opcode(line, line_label_type);

        /**
         * ---------------------
         * Insert line number and opcode binary
         * ---------------------
         */
        strcat(file_res, get_output_line_counter(1));
        strcat(file_res, " ");

        /**
         * ---------------------
         * Handle opcode
         * ---------------------
         */
        updated_exit_code = handle_opcode(line_number, line_res, opcode);
        exit_code = update_exit_code(exit_code, updated_exit_code);

        /**
         * ---------------------
         * Handle operands for opcode output line
         * ---------------------
         */
        updated_exit_code = handle_opcode_operands(line_number, line_res, line,
                                                   opcode, line_label_type);
        exit_code = update_exit_code(exit_code, updated_exit_code);

        /* Handle ARE - opcode output line is always 100 */
        strcat(line_res, "100");

        /* Cast the current binary to octal */
        helper = cast_binary_to_octal(line_res);
        strcat(file_res, helper);

        free(helper);
        helper = NULL;

        /* Start creating the new line of the output */
        strcat(file_res, "\n");

        free(line_res);
        line_res = NULL;

        /**
         * ---------------------
         * Handle output per operand
         *
         * @note
         * This function can add to the output 0-2 lines
         * The function itself convert the output to octal, and enter a new line
         * ---------------------
         */
        line_res =
            handle_operands_output(line_number, line, opcode, line_label_type);
        if (line_res == NULL) {
            printf("line: %d, Error: Could not handle operands\n", line_number);
            exit_code = EXIT_FAILURE;
        }

        if (exit_code == EXIT_SUCCESS) {
            strcat(file_res, line_res);
        }

        free(line_res);
        line_res = NULL;

        free(opcode);
        opcode = NULL;
    }

    return file_res;
}

/**
 * Append the entry file with th output labels
 */
static void append_entry_file(OutputLabel *entry_label, String file_path,
                              FILE *file) {
    int i = 0;
    int size = get_string_array_length(entry_label->line_addresses);
    String output;

    if (entry_label->type == OUTPUT_LABEL_EXTERN) {
        return;
    }

    for (i = 0; i < size; i++) {
        output = pad_left(entry_label->line_addresses[i], 4, '0');

        fprintf(file, "%s %s\n", entry_label->name, output);

        free(output);
        output = NULL;
    }
}

/**
 * Append the extern file with the output labels
 */
static void append_extern_file(OutputLabel *extern_label, String file_path,
                               FILE *file) {
    int i = 0;
    int size = get_string_array_length(extern_label->line_addresses);

    if (extern_label->type == OUTPUT_LABEL_ENTRY) {
        return;
    }

    for (i = 0; i < size; i++) {
        fprintf(file, "%s %s\n", extern_label->name,
                extern_label->line_addresses[i]);
    }
}

/**
 * Append the entry file with labels that does not appear as operands, but
 * defined
 */
static void append_non_used_entries(Label *label, String file_path,
                                    FILE *file) {
    String output;
    String helper;

    if (label->has_entry == 0 || label->is_defined == 0) {
        return;
    }

    output = cast_decimal_to_string(label->memory_address + SYMBOL_START_POINT);
    helper = pad_left(output, 4, '0');

    if (has_output_label(label->name) == 0) {
        fprintf(file, "%s %s\n", label->name, helper);
    }

    free(output);
    output = NULL;

    free(helper);
    helper = NULL;
}

/**
 * Create the entry / extern files
 *
 * @param file_paths the paths for the files, after the post-process
 */
static void create_entry_extern_files(String file_path) {
    String extern_target_file_path =
        replace_substring(file_path, (String)POST_PROCESS_FILE_EXTENSION,
                          (String)EXTERN_FILE_EXTENSION);

    String entry_target_file_path =
        replace_substring(file_path, (String)POST_PROCESS_FILE_EXTENSION,
                          (String)ENTRY_FILE_EXTENSION);

    FILE *entry_file;
    FILE *extern_file;

    entry_file = fopen(entry_target_file_path, "w");
    if (entry_file == NULL) {
        printf("Error: Could not create entry file '%s'\n", file_path);
        return;
    }

    /**
     * Print labels with entries but not in use
     */
    iterate_labels(append_non_used_entries, entry_target_file_path, entry_file);

    iterate_output_labels(append_entry_file, entry_target_file_path,
                          entry_file);
    fclose(entry_file);

    extern_file = fopen(extern_target_file_path, "w");
    if (extern_file == NULL) {
        printf("Error: Could not create extern file '%s'\n", file_path);
        return;
    }

    iterate_output_labels(append_extern_file, extern_target_file_path,
                          extern_file);
    fclose(extern_file);

    free(entry_target_file_path);
    entry_target_file_path = NULL;
}

/**
 * Handle the output for the files
 *
 * @param file_paths the paths for the files, after the post-process
 *
 * @return EXIT_SUCCESS if the output was successful, otherwise EXIT_FAILURE
 */
int handle_output(String *file_paths) {
    FILE *file;
    int is_failed = EXIT_SUCCESS;
    String file_res;
    int i;

    String target_file_path;

    /**
     * Create output files
     */
    for (i = 0; file_paths[i] != NULL; i++) {
        file_res = generate_file_output(file_paths[i]);

        if (file_res == NULL) {
            printf("Error: Could not create output for file: '%s' \n",
                   file_paths[i]);

            is_failed = EXIT_FAILURE;
            continue;
        }

        target_file_path = replace_substring(
            file_paths[i], (String)POST_PROCESS_FILE_EXTENSION,
            (String)TARGET_FILE_EXTENSION);

        file = fopen(target_file_path, "w");
        if (file == NULL) {
            printf("Error: Could not create target file '%s'\n",
                   target_file_path);
            is_failed = EXIT_FAILURE;

            free(file_res);
            file_res = NULL;

            free(target_file_path);
            target_file_path = NULL;
            continue;
        }

        /* Add the IC / DC at the top */
        fprintf(file, "  %d %d\n", get_instruction_counter(0),
                get_data_counter(0));

        fprintf(file, "%s", file_res);
        fclose(file);

        create_entry_extern_files(file_paths[i]);

        free(file_res);
        file_res = NULL;

        free(target_file_path);
        target_file_path = NULL;
    }

    return is_failed;
}
