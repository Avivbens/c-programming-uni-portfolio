#include "command.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static String opcodes[OPCODES_NUM] = {
    (String) "mov", (String) "cmp", (String) "add", (String) "sub",
    (String) "lea", (String) "clr",

    (String) "not", (String) "inc", (String) "dec", (String) "jmp",
    (String) "bne", (String) "red",

    (String) "prn", (String) "jsr", (String) "rts", (String) "stop"};

/**
 * Get the number of operands for a given opcode
 *
 * @param word The opcode to check
 *
 * @returns - amount of required operands, -1 if not a valid opcode
 * @throw EXIT_FAILURE if the opcode does not define amount of operands
 */
static int get_operands_number_per_opcode(String word) {
    int opcode_index = is_command(trim_string(word));

    /**
     * Not a command
     */
    if (opcode_index == -1) {
        return -1;
    }

    /**
     * Determine the number of operands the opcode can receive
     */
    if (opcode_index >= 0 && opcode_index <= 4) {
        return 2;
    }

    if (opcode_index >= 5 && opcode_index <= 13) {
        return 1;
    }

    if (opcode_index >= 14 && opcode_index <= 15) {
        return 0;
    }

    printf("Error: operand %s does not have operands defined!\n", word);
    exit(EXIT_FAILURE);
}

/**
 * Check if the given token is a valid command
 *
 * @param word The token to check
 *
 * @returns the command index, or -1 if the token is not a command
 */
int is_command(String word) {
    int i;
    int length = strlen(word);
    if (length > MAX_COMMAND_LENGTH || length < MIN_COMMAND_LENGTH) {
        return -1;
    }

    for (i = 0; i < OPCODES_NUM; i++) {
        if (strcmp(word, opcodes[i]) == 0) {
            return i;
        }
    }

    return -1;
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
