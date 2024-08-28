#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./cli/cli.h"

int main(void) {
    Set *SETA = create_set();
    Set *SETB = create_set();
    Set *SETC = create_set();
    Set *SETD = create_set();
    Set *SETE = create_set();
    Set *SETF = create_set();

    char *user_query = (char *)malloc(100 * sizeof(char));

    Set *sets[6];
    sets[0] = SETA;
    sets[1] = SETB;
    sets[2] = SETC;
    sets[3] = SETD;
    sets[4] = SETE;
    sets[5] = SETF;

    add_to_set(SETA, 1);
    add_to_set(SETB, 2);

    print_query_message();
    while ((user_query = fgets(user_query, 100, stdin)) != NULL) {
        handle_prompt(user_query, sets);

        print_query_message();
    }

    return 0;
}
