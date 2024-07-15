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
/**
 * Extracts the first word from a given line.
 *
 * This function scans the input line and returns the first word it encounters.
 * If the line is empty or starts with a space, it returns NULL. If memory
 * allocation for the first word fails, it logs an error and exits the program
 * with EXIT_FAILURE.
 *
 * @param line The input line from which to extract the first word.
 * @returns A dynamically allocated string containing the first word of the
 * line, or NULL if the line is empty or starts with a space.
 */

String get_first_word_from_line(const String line) {
    int length = strlen(line);
    int end = 0;
    while (end < length && line[end] != ' ' && line[end] != '\0') {
        end++;
    }
    if (end == 0) return NULL;
    String word = (String)malloc(sizeof(char) * (end + 1));
    if (word == NULL) {
        printf("Error: Could not allocate memory\n");
        exit(EXIT_FAILURE);
    }
    strncpy(word, line, end);
    word[end] = '\0';
    return word;
}