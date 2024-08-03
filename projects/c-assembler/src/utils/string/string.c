#include "string.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Replace a substring with another substring
 *
 * @param original The original string
 * @param to_replace The substring to replace
 * @param replacement The substring to replace with
 *
 * @returns A dynamically allocated string with the replaced substring
 * @throw If memory allocation fails, the program exits with EXIT_FAILURE.
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
 *
 * @param str The string to trim
 *
 * @returns The trimmed string
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
 * @param line The input line from which to extract the first word.
 * @returns A dynamically allocated string containing the first word of the
 * line, or NULL if the line is empty or starts with a space.
 *
 * @throw If memory allocation fails, the program exits with EXIT_FAILURE.
 */
String get_first_word_from_line(String line) {
    int length = strlen(line);
    String word;
    int end = 0;

    while (end < length && line[end] != ' ' && line[end] != '\0') {
        end++;
    }

    if (end == 0) {
        return NULL;
    }

    word = (String)malloc(sizeof(char) * (end + 1));
    if (word == NULL) {
        printf("Error: Could not allocate memory\n");
        exit(EXIT_FAILURE);
    }

    strncpy(word, line, end);
    word[end] = '\0';
    return word;
}

/**
 * Get a word in a line based on its position
 *
 * @param line The input line from which to extract the word
 * @param word_number The position of the word to extract
 *
 * @note
 * First word is at position 0
 */
String get_word(String line, int word_number) {
    String word;
    line = trim_string(line);

    /* Skip over the current word (non-space characters) */
    while (word_number > 0) {
        /* Skip current word */
        while (*line && !isspace((unsigned char)*line)) {
            line++;
        }

        /* Skip over the spaces */
        while (*line && isspace((unsigned char)*line)) {
            line++;
        }

        /**
         * Reached to the next word
         */

        /* End of String */
        if (*line == '\0') {
            return NULL;
        }

        word_number--;
    }

    /**
     * Create a copy of the word
     */
    word = get_first_word_from_line(line);
    return word;
}

/**
 * Check if a string starts with a given prefix
 *
 * @param str The string to check
 * @param prefix The prefix to check for
 *
 * @returns 1 if the string starts with the prefix, 0 otherwise
 */
int starts_with(String str, String prefix) {
    return strncmp(str, prefix, strlen(prefix)) == 0;
}

int ends_with(String str, String suffix) {
    int str_len = strlen(str);
    int suffix_len = strlen(suffix);

    if (suffix_len > str_len) {
        return 0;
    }

    return strncmp(str + str_len - suffix_len, suffix, suffix_len) == 0;
}
