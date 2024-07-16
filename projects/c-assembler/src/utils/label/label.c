#include "label.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

/**  function to determine if a string is a label
 * a label must:
 * start with an alphabet character(upper or lower)
 * followed by alphabets characters and or digits only
 * the max length is 31 characters
 * The last character must be ':' (and be max in the 32th index)
 * no spaces allowed at all */

int is_label(const char* str) {
    int i;
    int len = strlen(str);
    if (len > MAX_LABEL_LENGTH + 1) {
        return 0;
    }

    if (!isalpha(str[0])) {
        return 0;
    }

    if (str[len - 1] != ':') {
        return 0;
    }

    for (i = 1; i < len - 1; i++) {
        if (!isalnum(str[i])) {
            return 0;
        }
    }

    return 1;
}
