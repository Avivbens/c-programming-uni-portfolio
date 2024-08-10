#ifndef ARRAY_H
#define ARRAY_H

int get_array_length(void *array, int memory_size_per_cell);
void free_array_recursively(void **array, int length);

#endif
