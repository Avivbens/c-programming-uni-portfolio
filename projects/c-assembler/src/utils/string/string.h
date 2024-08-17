#include "../../constants/constants.h"

#ifndef STRING_H
#define STRING_H

#define String char*

String replace_substring(String original, String to_replace,
                         String replacement);
String pad_left(String str, int length, char padding);
String trim_string(String str);
String get_word(String line, int word_number);
int starts_with(String str, String prefix);
int ends_with(String str, String suffix);
String* split_string(String line, String delimiter);
String substring_words(String line, int word_number);
String cast_decimal_to_binary(String number);
String cast_binary_to_octal(String binary_form);
String cast_decimal_to_string(int number);

#endif
