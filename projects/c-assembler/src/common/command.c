#include "command.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    OpcodeBinary* opcode = get_command(trim_string(word));

    /**
     * Not a command
     */
    if (opcode == NULL) {
        return -1;
    }

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
