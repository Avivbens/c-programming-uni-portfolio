#include "../constants/constants.h"
#include "../utils/string/string.h"

#ifndef COMMAND_H
#define COMMAND_H

#define OPCODES_NUM 16

typedef enum {
    /*  */
    INVALID_OPERANDS = -1,
    NOT_EXISTS,
    VALID
} OpcodeCheck;

typedef struct OpcodeBinary {
    String opcode;
    String binary;
    int operands;
} OpcodeBinary;

OpcodeBinary* get_command(String word);
String generate_opcode_binary(String opcode);
int get_operands_number_per_opcode(String word);
OpcodeCheck validate_opcode_operand(String word, int operand_count);

#endif
