#include "../string/string.h"

#ifndef ARRAY_H
#define ARRAY_H

int get_string_array_length(void* array, int memory_size_per_cell);
void free_string_array_recursively(String* array, int length);

#endif
