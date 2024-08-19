#include "command.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define strdup _strdup
#endif

/**
 * TODO - check the allowed address modes
 */
static OpcodeBinary opcodes[OPCODES_NUM] = {
    {(String) "mov", (String) "0000", 2},
    {(String) "cmp", (String) "0001", 2},
    {(String) "add", (String) "0010", 2},
    {(String) "sub", (String) "0011", 2},
    {(String) "lea", (String) "0100", 2},
    {(String) "clr", (String) "0101", 1},
    {(String) "not", (String) "0110", 1},
    {(String) "inc", (String) "0111", 1},
    {(String) "dec", (String) "1000", 1},
    {(String) "jmp", (String) "1001", 1},
    {(String) "bne", (String) "1010", 1},
    {(String) "red", (String) "1011", 1},
    {(String) "prn", (String) "1100", 1},
    {(String) "jsr", (String) "1101", 1},
    {(String) "rts", (String) "1110", 0},
    {(String) "stop", (String) "1111", 0}};

/**
 * Get the number of operands for a given opcode
 *
 * @param word The opcode to check
 *
 * @returns - amount of required operands, -1 if not a valid opcode
 */
int get_operands_number_per_opcode(String word) {
    String parsed_word = trim_string(word);
    OpcodeBinary* opcode = get_command(parsed_word);

    /**
     * Not a command
     */
    if (opcode == NULL) {
        free(parsed_word);
        parsed_word = NULL;

        return -1;
    }

    free(parsed_word);
    parsed_word = NULL;

    return opcode->operands;
}

/**
 * Generate the binary representation of an opcode
 *
 * @param opcode The opcode to generate the binary for
 *
 * @returns the binary representation of the opcode or NULL if the opcode does
 * not exist
 */
String generate_opcode_binary(String opcode) {
    OpcodeBinary* opcode_binary = get_command(opcode);

    if (opcode_binary == NULL) {
        return NULL;
    }

    return opcode_binary->binary;
}

/**
 * Check if the given token is a valid command
 *
 * @param word The token to check
 *
 * @returns a pointer to the opcode if it exists, otherwise NULL
 */
OpcodeBinary* get_command(String word) {
    int i;
    int length = strlen(word);
    if (length > MAX_COMMAND_LENGTH || length < MIN_COMMAND_LENGTH) {
        return NULL;
    }

    for (i = 0; i < OPCODES_NUM; i++) {
        if (strcmp((const char*)word, opcodes[i].opcode) == 0) {
            return &opcodes[i];
        }
    }

    return NULL;
}

/**
 * Validate an opcode with the amount of operands exists
 *
 * @param word The opcode to validate
 * @param operand_count The amount of operands the opcode has
 *
 * @returns {@link OpcodeCheck}
 */
OpcodeCheck validate_opcode_operand(String word, int operand_count) {
    int required_operands = get_operands_number_per_opcode(word);

    /**
     * Not a command
     */
    if (required_operands == -1) {
        return NOT_EXISTS;
    }

    if (operand_count != required_operands) {
        return INVALID_OPERANDS;
    }

    return VALID;
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
String extract_operand(String line, LabelType label_type, int operand_index) {
    String helper;
    String operand;
    String* operands;
    int operands_prefix = label_type == NOT_LABEL ? 1 : 2;
    String operands_string = substring_words(line, operands_prefix);

    operands = split_string(operands_string, (String) ",");

    /* In case we've got just 1 item, split returns array with 1 cell */
    helper = operands[operand_index];
    if (helper == NULL) {
        helper = operands[operand_index - 1];
    }

    operand = trim_string(helper);

    return operand;
}

/**
 * Check if all operands are registers
 *
 * @param line the line to check
 * @param label_type the label type
 * @param operand_count the number of operands
 *
 * @returns 1 if all operands are registers, 0 otherwise
 */
int is_all_operands_are_registers(String line, LabelType label_type,
                                  int operand_count) {
    String operand = NULL;
    AddressMode address_mode;

    int i;

    if (operand_count < 2) {
        return 0;
    }

    for (i = 0; i < operand_count; i++) {
        operand = extract_operand(line, label_type, i);
        address_mode = get_address_mode(operand);

        free(operand);
        operand = NULL;

        if (address_mode != DIRECT_ACCUMULATED_ADDRESS_MODE &&
            address_mode != INDIRECT_ACCUMULATED_ADDRESS_MODE) {
            return 0;
        }
    }

    return 1;
}

/**
 * Generate the address mode for an operand
 *
 * @param operand the operand to generate the address mode for
 *
 * @returns the address mode for the operands
 */
AddressMode get_address_mode(String raw_operand) {
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
