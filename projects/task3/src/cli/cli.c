#include "./cli.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/utils.h"

void print_query_message(void) {
    printf("Enter a query in the following format: SETA ADD 5\n");
}

/**
 * Get set number by name
 * @param sets - array of sets
 * @param set_name - name of the set
 * @returns the set, NULL if the set name is invalid
 */
Set *parse_set_name(Set *sets[], char *set_name) {
    if (!strcmp(set_name, "SETA")) {
        return sets[0];
    }

    if (!strcmp(set_name, "SETB")) {
        return sets[1];
    }

    if (!strcmp(set_name, "SETC")) {
        return sets[2];
    }

    if (!strcmp(set_name, "SETD")) {
        return sets[3];
    }

    if (!strcmp(set_name, "SETE")) {
        return sets[4];
    }

    if (!strcmp(set_name, "SETF")) {
        return sets[5];
    }

    /**
     * No matching set name
     */
    return NULL;
}

int handle_prompt(char *user_query, Set *sets[]) {
    Set *targeted_set_1, *targeted_set_2;
    char *handler;

    handler = extract_handler(user_query, (char *)(" "), 0);
    if (handler == NULL) {
        printf("Invalid command\n");
        return 1;
    }

    targeted_set_1 = parse_set_name(sets, handler);
    if (targeted_set_1 == NULL) {
        printf("Invalid set name - %s\n", handler);
        return 1;
    }

    handler = extract_handler(user_query, (char *)(" "), 1);
    if (handler == NULL) {
        printf("Invalid command\n");
        return 1;
    }

    targeted_set_2 = parse_set_name(sets, handler);
    if (targeted_set_2 == NULL) {
        printf("Invalid set name - %s\n", handler);
        return 1;
    }

    printf("handler: %s\n", handler);

    print_set(targeted_set_1);
    print_set(targeted_set_2);

    return 0;
}
