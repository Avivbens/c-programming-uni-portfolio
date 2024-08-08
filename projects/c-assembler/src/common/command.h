#include "../constants/constants.h"
#include "../utils/string/string.h"

#ifndef COMMAND_H
#define COMMAND_H

#define OPCODES_NUM 16

typedef enum {
    /*  */
    INVALID_OPERANDS = -1,
    NOT_EXISTS,
    VALID,
} OpcodeCheck;

String opcodes[OPCODES_NUM] = {(String) "mov", (String) "cmp", (String) "add",
                               (String) "sub", (String) "lea", (String) "clr",

                               (String) "not", (String) "inc", (String) "dec",
                               (String) "jmp", (String) "bne", (String) "red",

                               (String) "prn", (String) "jsr", (String) "rts",
                               (String) "stop"};

int is_command(String word);
OpcodeCheck validate_opcode_operand(String word, int operand_count);

#endif
