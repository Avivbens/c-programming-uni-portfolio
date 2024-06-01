#include "./utils.h"

#include <stdlib.h>
#include <string.h>

/**
 * Get handler by index, devided by spaces
 * @param string - string to extract the handler from
 * @param devider - devider to split the string by
 * @param index - index of the results to extract
 */
char *extract_handler(char *string, char *devider, int index) {
    int i;
    size_t len;
    char *handler;
    char *saveptr;

    char *clone = (char *)malloc(strlen(string) * sizeof(char));
    strcpy(clone, string);

    handler = strtok_r(clone, devider, &saveptr);
    if (handler == NULL) {
        return NULL;
    }

    if (index != 0) {
        for (i = 0; i < index; i++) {
            handler = strtok_r(NULL, devider, &saveptr);
            if (handler == NULL) {
                return NULL;
            }
        }
    }

    len = strlen(handler) - 1;
    if (*handler && handler[len] == '\n') {
        handler[len] = '\0';
    }

    return handler;
}
