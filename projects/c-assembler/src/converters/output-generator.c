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
static String get_instruction_counter(int increment) {
    static int instruction_counter = IC_OUTPUT_START_POINT;
    int res = instruction_counter;
    String str_res;

    instruction_counter += increment;

    str_res = cast_decimal_to_string(res);
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
 * Extract the operand from a line
 *
 * @attention - free this memory after use
 *
 * @param line the line to extract the operand from
 * @param label_type the label type of the line
 * @param operand_index the index of the operand to extract, starts from 0
 *
 * @returns the operand
 */
static String extract_operand(String line, LabelType label_type,
                              int operand_index) {
    String helper;
    String operand;
    String *operands;
    int operands_prefix = label_type == NOT_LABEL ? 1 : 2;
    String operands_string = substring_words(line, operands_prefix);

    operands = split_string(operands_string, (String) ",");

    helper = operands[operand_index];
    operand = trim_string(helper);

    return operand;
}

/**
 * Generate the address mode for an operand
 *
 * @param operand the operand to generate the address mode for
 *
 * TODO - what about just numbers, without the #?
 *
 * @returns the address mode for the operands
 */
static AddressMode get_address_mode(String raw_operand) {
    String operand = trim_string(raw_operand);
    String helper;

    if (starts_with(operand, (String) "#")) {
        if (is_number(operand + 1)) {
            return IMMEDIATE_ADDRESS_MODE;
        }

        return ERROR;
    }

    /**
     * Indirect access to registers
     */
    if (starts_with(operand, (String) "*")) {
        /**
         * Validate the operand is a register
         */
        helper = replace_substring(operand, (String) "*", (String) "");
        if (!is_register(helper)) {
            free(helper);
            helper = NULL;

            return ERROR;
        }

        free(helper);
        helper = NULL;
        return INDIRECT_ACCUMULATED_ADDRESS_MODE;
    }

    /**
     * In case the operands are a label, we need to check if it exists
     */
    if (has_label(operand)) {
        return DIRECT_ADDRESS_MODE;
    }

    /**
     * Validate the operand is a register
     */
    if (is_register(operand)) {
        return DIRECT_ACCUMULATED_ADDRESS_MODE;
    }

    return ERROR;
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

static int generate_file_output(String file_path) {
    FILE *file;
    int exit_code = EXIT_SUCCESS;
    char line[MAX_LINE_LENGTH];
    int line_number = 0;
    int operand_count;
    int i;

    String opcode = NULL;
    String operand = NULL;
    String opcode_binary;
    LabelType line_label_type;
    AddressMode address_mode;

    String helper;

    String line_res = NULL;

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

        line_label_type = is_label(line);
        line_res =
            (String)malloc(MAX_LINE_OUTPUT * MAX_LINE_LENGTH * sizeof(char));
        if (line_res == NULL) {
            printf("Error: Could not allocate memory for line\n");
            exit(EXIT_FAILURE);
        }

        /**
         * Handle first 4 digits of binary code
         */
        opcode = extract_opcode(line, line_label_type);
        operand_count = get_operands_number_per_opcode(opcode);
        opcode_binary = generate_opcode_binary(opcode);
        if (opcode_binary == NULL || operand_count == -1) {
            printf("line: %d, Error: Invalid opcode - '%s'\n", line_number,
                   opcode);
            exit_code = EXIT_FAILURE;
            continue;
        }

        /**
         * ---------------------
         * Insert line number and opcode binary (as octal)
         * ---------------------
         */
        strcat(line_res, get_instruction_counter(1));
        strcat(line_res, " ");

        helper = cast_binary_to_octal(opcode_binary);
        strcat(line_res, helper);

        free(helper);
        helper = NULL;

        /**
         * TODO - add:
         * 1 - 4 digits of source operand
         * 2 - 4 digits of destination operand
         * 3 - ARE
         */

        /**
         * Handle all the operands
         */
        for (i = 0; i < 2; i++) {
            if (i >= operand_count) {
                /**
                 * Insert empty data into the results, in case there is no
                 * operand
                 */

                strcat(line_res, "0000");
                break;
            }

            operand = extract_operand(line, line_label_type, i);
            address_mode = get_address_mode(operand);

            /**
             * ---------------------
             * The operand address mode should be:
             *
             * 0 - numbers, start with #. We need to cast it to binary. Would be
             * the entire binary, but the last 3 digits (ARE)
             *
             * 1 - labels, show the address of the label (based on the labels
             * system). Would be the entire binary, but the last 3 digits (ARE)
             *
             * 2 - pointer to register, show the register number. Would be the
             * entire binary, but the last 3 digits (ARE)
             *
             * 3 - register, show the `3` in binary. Would be the entire binary,
             * but the last 3 digits (ARE)
             *
             *
             * These need to be correlated to the operand itself (source /
             * dest).
             *
             * The source should be from 6-8 bits, the dest should be 3-5 bits.
             * ---------------------
             */
            if (address_mode == ERROR) {
                printf("line: %d, Error: Invalid operand - '%s'\n", line_number,
                       operand);
                exit_code = EXIT_FAILURE;

                free(operand);
                operand = NULL;

                break;
            }

            helper = get_binary_by_address_mode(address_mode);
            strcat(line_res, helper);

            free(operand);
            operand = NULL;
        }

        /**
         * Handle ARE - this is an opcode
         */
        strcat(line_res, "100");

        /* Start creating the new line of the output */
        strcat(line_res, "\n");

        /**
         * Handle operands lines
         */
        for (i = 0; i < operand_count; i++) {
            strcat(line_res, get_instruction_counter(1));
            strcat(line_res, " ");

            /* Inert empty value instead of the opcode */
            strcat(line_res, "0000");

            operand = extract_operand(line, line_label_type, i);
            address_mode = get_address_mode(operand);

            if (address_mode == ERROR) {
                printf("line: %d, Error: Invalid operand - '%s'\n", line_number,
                       operand);
                exit_code = EXIT_FAILURE;

                free(operand);
                operand = NULL;

                break;
            }

            helper = cast_decimal_to_binary(address_mode);
            strcat(line_res, helper);

            free(operand);
            operand = NULL;

            free(helper);
            helper = NULL;
        }
    }

    return exit_code;
}

/**
 * Handle the output for the files
 *
 * @param file_paths the paths for the files
 *
 * @return EXIT_SUCCESS if the output was successful, otherwise EXIT_FAILURE
 */
int handle_output(String *file_paths) {
    int i;
    int is_failed = EXIT_SUCCESS;
    int create_output_res = EXIT_SUCCESS;

    /**
     * Create output files
     */
    for (i = 0; file_paths[i] != NULL; i++) {
        create_output_res = generate_file_output(file_paths[i]);

        if (create_output_res == EXIT_FAILURE) {
            printf("Error: Could not create output for file: '%s' \n",
                   file_paths[i]);

            is_failed = EXIT_FAILURE;
        }
    }

    return is_failed;
}
