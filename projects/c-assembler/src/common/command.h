#include "../constants/constants.h"
#include "../data/symbols-data.h"
#include "../utils/number/number.h"
#include "../utils/string/string.h"
#include "./register.h"

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

typedef enum {
    ERROR = -1,
    IMMEDIATE_ADDRESS_MODE = 0,
    DIRECT_ADDRESS_MODE = 1,
    INDIRECT_ACCUMULATED_ADDRESS_MODE = 2,
    DIRECT_ACCUMULATED_ADDRESS_MODE = 3
} AddressMode;

OpcodeBinary* get_command(String word);
String generate_opcode_binary(String opcode);
int get_operands_number_per_opcode(String word);
OpcodeCheck validate_opcode_operand(String word, int operand_count);

String extract_operand(String line, LabelType label_type, int operand_index);
int is_all_operands_are_registers(String line, LabelType label_type,
                                  int operand_count);
AddressMode get_address_mode(String raw_operand);

#endif
