#include "string.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Extracts the first word from a given line.
 *
 * @attention - free this memory after use
 *
 * @param line The input line from which to extract the first word.
 * @returns A dynamically allocated string containing the first word of the
 * line, or NULL if the line is empty or starts with a space.
 *
 * @throw If memory allocation fails, the program exits with EXIT_FAILURE.
 */
static String get_first_word_from_line(String line) {
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
        printf("Error(get_first_word_from_line): Could not allocate memory\n");
        exit(EXIT_FAILURE);
    }

    strncpy(word, line, end);
    word[end] = '\0';
    return word;
}

/**
 * Pad a string to the left with a given character
 *
 * @attention - free this memory after use
 *
 * @param str The string to pad
 * @param length The length to pad the string to
 * @param padding The character to pad the string with
 *
 * @returns A string containing the padded string with the fixed length
 */
String pad_left(String str, int length, char padding) {
    int str_len = strlen(str);
    int max_size = str_len > length ? str_len : length;
    String padded_str = (String)malloc(sizeof(char) * (max_size + 1));
    int i;

    if (padded_str == NULL) {
        printf("Error(pad_left): Could not allocate memory\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < length - str_len; i++) {
        padded_str[i] = padding;
    }

    strncpy(padded_str + i, str, str_len);
    padded_str[length] = '\0';

    return padded_str;
}

/**
 * Extracts string from a given position by words
 *
 * @attention - free this memory after use
 *
 * @param line The input line from which to extract the word
 * @param word_number The position of the word to extract
 */
String substring_words(String line, int word_number) {
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
     * Create a copy of the rest
     */
    word = (String)malloc(strlen(line) + 1);
    if (word == NULL) {
        printf("Error(substring_words): Could not allocate memory\n");
        exit(EXIT_FAILURE);
    }

    strcpy(word, line);
    return word;
}

/**
 * Replace a substring with another substring
 *
 * @attention - free this memory after use
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
    String temp = strdup(original);
    int count = 0;
    int to_replace_len = strlen(to_replace);
    int replacement_len = strlen(replacement);
    int new_str_len;

    String new_str;
    String helper_ptr;

    /* Count occurrences of the substring to replace */
    while ((temp = strstr(temp, to_replace))) {
        count++;
        temp += to_replace_len;
    }

    /* Allocate memory for the new string */
    new_str_len =
        strlen(original) + (replacement_len - to_replace_len) * count + 1;
    new_str = (String)malloc(new_str_len);

    if (!new_str) {
        printf(
            "Error(replace_substring): Could not allocate `new_str` memory\n");
        return NULL;
    }

    helper_ptr = new_str;
    while (*original) {
        /* If the substring matches, replace it */
        if (strstr(original, to_replace) == original) {
            memcpy(helper_ptr, replacement, replacement_len);
            helper_ptr += replacement_len;
            original += to_replace_len;
        } else {
            *helper_ptr++ = *original++;
        }
    }

    /* Null-terminate the new string */
    *helper_ptr = '\0';

    return new_str;
}

/**
 * Trim leading and trailing spaces from a string
 *
 * @attention - free this memory after use
 *
 * @param str The string to trim
 *
 * @returns A new trimmed string
 */
String trim_string(String str) {
    String new_str;
    String end;

    if (str == NULL) {
        return NULL;
    }

    new_str = (String)malloc(strlen(str) + 1);
    if (new_str == NULL) {
        printf("Error(trim_string): Could not allocate memory\n");
        exit(EXIT_FAILURE);
    }

    /* Trim leading space */
    while (isspace((unsigned char)*str)) {
        str++;
    }

    /* Trim trailing space */
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }

    /* Put new string into the new_str */
    strncpy(new_str, str, end - str + 1);
    new_str[end - str + 1] = '\0';

    return new_str;
}

/**
 * Get a word in a line based on its position
 *
 * @attention - free this memory after use
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

/**
 * Check if a string ends with a given suffix
 *
 * @param str The string to check
 * @param suffix The suffix to check for
 *
 * @returns 1 if the string ends with the suffix, 0 otherwise
 */
int ends_with(String str, String suffix) {
    int str_len = strlen(str);
    int suffix_len = strlen(suffix);

    if (suffix_len > str_len) {
        return 0;
    }

    return strncmp(str + str_len - suffix_len, suffix, suffix_len) == 0;
}

/**
 * Remove the quotation marks from a string
 *
 * @attention - free this memory after use
 *
 * @param origin The string to remove the quotation marks from
 * @returns A string without the quotation marks
 */
String remove_quotation(String origin) {
    int length = strlen(origin);
    String new_string;

    /* Check if the string starts and ends with quotes */
    if (length < 3 || origin[0] != '"' || origin[length - 1] != '"') {
        printf("Error(remove_quotation): Invalid string structure\n");
        return NULL;
    }

    new_string = (String)malloc(length - 1);

    if (new_string == NULL) {
        printf("Error(remove_quotation): failed to allocate memory for %s\n",
               new_string);
        exit(EXIT_FAILURE);
    }

    /**
     * Copy just the relevant string parts
     */
    strncpy(new_string, origin + 1, length - 2);
    new_string[length - 2] = '\0';

    return new_string;
}

/**
 * Split a string into tokens based on a delimiter
 *
 * @attention - free this memory after use (array and each element)
 *
 * @param line The string to split
 * @param delimiter The delimiter to split the string by
 *
 * @returns An array of strings containing the tokens
 */
String* split_string(String line, String delimiter) {
    String* tokens = NULL;
    String token = strtok(line, delimiter);
    int i = 0;

    while (token != NULL) {
        tokens = (String*)realloc(tokens, sizeof(String) * (i + 1));
        if (tokens == NULL) {
            printf("Error(split_string): failed to allocate memory\n");
            exit(EXIT_FAILURE);
        }

        tokens[i] = token;
        token = strtok(NULL, delimiter);
        i++;
    }

    tokens[i] = (String)'\0';
    return tokens;
}

/**
 * Cast a decimal number to a binary string
 *
 * @attention - free this memory after use
 *
 * @param number The decimal number to cast to binary
 *
 * @returns A string containing the binary representation of the number
 */
String cast_to_binary(String number) {
    int num = atoi(number);
    int i = 0;
    String binary = (String)malloc((32 + 1) * sizeof(char));

    if (binary == NULL) {
        printf("Error(cast_to_binary): Could not allocate memory\n");
        exit(EXIT_FAILURE);
    }

    for (i = 32 - 1; i >= 0; i--) {
        binary[i] = (num & 1) ? '1' : '0';
        num >>= 1;
    }

    binary[32] = '\0';

    return binary;
}
