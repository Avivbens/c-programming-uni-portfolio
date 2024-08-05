#include "../constants/constants.h"
#include "../utils/string/string.h"

#ifndef COMMAND_H
#define COMMAND_H

int is_command(String word);
static char* opcodes[OPCODES_NUM] = {"mov", "cmp", "add", "sub", "lea", "clr",
                                     "not", "inc", "dec", "jmp", "bne", "red",
                                     "prn", "jsr", "rts", "stop"};

#endif
