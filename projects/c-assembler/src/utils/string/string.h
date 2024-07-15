#ifndef String
#define String char*
#endif

String replace_substring(String original, String to_replace,
                         String replacement);
String trim_string(String str);
String get_first_word_from_line(const String line);
