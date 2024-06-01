#include <stdio.h>
#include <stdlib.h>

#include "./set/set.h"

int main(void) {
    Set *set = create_set();

    add_to_set(set, 5);
    add_to_set(set, 33);
    add_to_set(set, 89);
    add_to_set(set, 127);

    print_set(set);

    remove_from_set(set, 33);

    print_set(set);

    free(set);

    return 0;
}
