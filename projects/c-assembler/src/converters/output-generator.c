#include "output-generator.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Filter out lines with Label types
 *
 * We only need to create a binary row for not labels / labels without a type
 *
 * @returns 1 if the line should be processed, 0 if it should be skipped
 */
static int filter_line(String line, LabelType label_type) {
    if (starts_with(trim_string(line), (String) ".data")) {
        return 0;
    }

    if (label_type == NOT_LABEL || label_type == NOT_LABEL_TYPE) {
        return 1;
    }

    return 0;
}

/**
 * Extract the opcode from a line
 *
 * @param line the line to extract the opcode from
 *
 * @returns the opcode name
 *
 * @note if the line is a label, the opcode is the second word
 */
static String extract_opcode(String line, LabelType label_type) {
    String opcode;

    if (label_type == NOT_LABEL) {
        opcode = get_word(line, 0);
        return trim_string(opcode);
    }

    opcode = get_word(line, 1);
    return trim_string(opcode);
}

/**
 * Extract the operand from a line
 *
 * @param line the line to extract the operand from
 * @param label_type the label type of the line
 * @param operand_index the index of the operand to extract, starts from 0
 *
 * @returns the operand
 */
static String extract_operand(String line, LabelType label_type,
                              int operand_index) {
    String operand;
    String *operands;
    int operands_prefix = label_type == NOT_LABEL ? 1 : 2;
    String operands_string = substring_words(line, operands_prefix);

    operands = split_string(operands_string, (String) ",");
    operand = operands[operand_index];

    return trim_string(operand);
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
AddressMode get_address_mode(String operand) {
    operand = trim_string(operand);

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
        if (!is_register(
                replace_substring(operand, (String) "*", (String) ""))) {
            return ERROR;
        }

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

static int generate_file_output(String file_path) {
    FILE *file;
    int exit_code = EXIT_SUCCESS;
    char line[MAX_LINE_LENGTH];
    int line_number = 0;
    int operand_count;
    int i;

    String opcode;
    String opcode_binary;
    String operand;
    LabelType line_label_type;
    AddressMode address_mode;

    String helper;

    String line_res;

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
        if (filter_line(line, line_label_type) == 0) {
            continue;
        }

        line_res = (String)malloc(MAX_LINE_OUTPUT * sizeof(char));
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
            ;
        }

        strcat(line_res, opcode_binary);

        /**
         * Handle all the operands
         */
        for (i = 0; i < 2; i++) {
            if (i >= operand_count) {
                /**
                 * Insert empty data into the results
                 */

                strcat(line_res, "0000");
                break;
            }

            operand = extract_operand(line, line_label_type, i);
            address_mode = get_address_mode(operand);

            if (address_mode == ERROR) {
                printf("line: %d, Error: Invalid operand - '%s'\n", line_number,
                       operand);
                exit_code = EXIT_FAILURE;
                break;
            }

            helper = (String)malloc(sizeof(char) * 5);
            if (helper == NULL) {
                printf("Error: Could not allocate memory for helper\n");
                exit(EXIT_FAILURE);
            }

            sprintf(helper, "%d", address_mode);

            /**
             * TODO - might be able to drop `pad_left`
             */
            strcat(line_res, pad_left(cast_to_binary(helper), 4, '0'));
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
