#include "command.h"

#include <string.h>

/**
 * Check if the given token is a valid command
 *
 * TODO - full implementation with whitelist
 *
 * @param word The token to check
 *
 * @returns 1 if the token is a command, 0 otherwise
 */
int is_command(String word) {
    int i;
    int length = strlen(word);
    if (length > MAX_COMMAND_LENGTH || length < MIN_COMMAND_LENGTH) {
        return 0;
    }
    for (i = 0; i < OPCODES_NUM; i++) {
        if (strcmp(word, opcodes[i]) == 0) {
            return 1;
        }
    }

    return 0;
}

int is_number(String token) {
    if (token[0] != '#') {
        return 0;
    }

    if (token[1] == '-' || token[1] == '+') {
        return isdigit(token[2]) != 0;
    } else {
        return isdigit(token[1]) != 0;
    }
}

int is_symbol(String token) {
    for (int i = 0; token[i] != '\0'; i++) {
        if (!isupper(token[i])) {
            return 0;
        }
    }
    return 1;
}

/**
 * Function to count valid operands in the string starting from a specific index
 *
 * @param line The line we are checking
 * @param start_word_index The word from which we start performing the test
 *
 * @returns the count of operands in the tested part.
 *  or -1 if the structure of the operands is incorrect or if there is more than
 * one comma between words
 */

int count_operands(const char* line, int start_word_index) {
    int count = 0;
    int word_number = start_word_index;
    int in_operand = 0;     /* 0 means false, 1 means true */
    int last_was_comma = 1; /* Start assuming position before the first
                               character is like having a comma */
    String current_word;

    while ((current_word = get_word((char*)line, word_number)) != NULL) {
        /* Check if the current word is valid (register, number, or label) */
        if (is_register(current_word) || is_number(current_word) ||
            is_symbol(current_word)) {
            count++;
            word_number++;
            in_operand = 1;
        } else {
            /* Invalid word found */
            free(current_word);
            return -1;
        }

        /* Check if there's exactly one comma after the word*/
        const char* after_word =
            line + (current_word - line) + strlen(current_word);
        while (isspace((unsigned char)*after_word)) {
            after_word++;
        }

        if (*after_word == ',') {
            last_was_comma = 1;
            after_word++; /* Move past the comma*/
            while (isspace((unsigned char)*after_word)) {
                after_word++;
            }
        } else {
            last_was_comma = 0;
        }

        /* Update line to start after the last valid comma */
        line = after_word;
        free(current_word);

        /* Check if we reached the end of the line*/
        if (*line == '\0') {
            break;
        }
    }

    /* If the last character processed was a comma, it's an error*/
    if (last_was_comma) {
        return -1;
    }

    return count;
}

int is_command(const char* word) {
    for (int i = 0; i < OPCODES_NUM; i++) {
        if (strcmp(word, opcodes[i]) == 0) {
            return i;  // Return the index of the opcode in the array
        }
    }
    return -1;  // Return -1 if the word is not an opcode
}

/* Function to check if a word is an opcode and validate its operand count */
int validate_opcode_operand(const char* word, int operand_count) {
    int opcode_index = is_command(word);

    if (opcode_index == -1) {
        return 0;  // Word is not an opcode
    }

    /* Determine the number of operands the opcode can receive*/
    int required_operands = 0;

    if (opcode_index >= 0 && opcode_index <= 4) {
        required_operands = 2; /* Opcodes 0-4 accept 2 operands*/
    } else if (opcode_index >= 5 && opcode_index <= 13) {
        required_operands = 1; /* Opcodes 5-13 accept 1 operand*/
    } else if (opcode_index >= 14 && opcode_index <= 15) {
        required_operands = 0; /* Opcodes 14-15 accept 0 operands*/
    }
    /* Compare the provided operand count with the expected operand count */
    if (operand_count == required_operands) {
        return 1; /* Valid opcode with correct operand count*/
    } else {
        return -1; /* Invalid operand count for the opcode*/
    }
}
