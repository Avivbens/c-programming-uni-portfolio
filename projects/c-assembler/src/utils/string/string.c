#include "string.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Replace a substring with another substring
 */
String replace_substring(String original, String to_replace,
                         String replacement) {
    String temp = original;
    int count = 0;
    int toReplaceLen = strlen(to_replace);
    int replacementLen = strlen(replacement);
    int newStrLen;

    String newStr;
    String helperPtr;

    /* Count occurrences of the substring to replace */
    while ((temp = strstr(temp, to_replace))) {
        count++;
        temp += toReplaceLen;
    }

    /* Allocate memory for the new string */
    newStrLen = strlen(original) + (replacementLen - toReplaceLen) * count + 1;
    newStr = (String)malloc(newStrLen);

    if (!newStr) {
        printf("Error: Could not allocate `newStr` memory\n");
        return NULL;
    }

    helperPtr = newStr;
    while (*original) {
        /* If the substring matches, replace it */
        if (strstr(original, to_replace) == original) {
            memcpy(helperPtr, replacement, replacementLen);
            helperPtr += replacementLen;
            original += toReplaceLen;
        } else {
            *helperPtr++ = *original++;
        }
    }

    /* Null-terminate the new string */
    *helperPtr = '\0';

    return newStr;
}

/**
 * Trim leading and trailing spaces from a string
 */
String trim_string(String str) {
    String end;

    /* Trim leading space */
    while (isspace((unsigned char)*str)) {
        str++;
    }

    if (*str == '\0') {
        return str;
    }

    /* Trim trailing space */
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }

    /* Write new null terminator */
    *(end + 1) = '\0';

    return str;
}
