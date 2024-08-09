#include "../utils/list/list.h"
#include "../utils/string/string.h"

#ifndef MACROS_DATA_H
#define MACROS_DATA_H

int add_macro(String name, String value);
String get_macro(String name);
int has_macro(String name);

void debug_macros(void);

#endif
