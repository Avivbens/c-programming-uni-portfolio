#ifndef String
#define String char*
#endif
#include "../../constants/constants.h"

String replace_substring(String original, String to_replace,
                         String replacement);
String trim_string(String str);
String get_first_word_from_line(String line);
String skip_to_next_word(String line);
