#include "../utils/file/file.h"
#include "../utils/string/string.h"

#ifndef CLI_PARSER_H
#define CLI_PARSER_H

#define ORIGINAL_FILE_EXTENSION ".as"
#define POST_PROCESS_FILE_EXTENSION ".am"
#define TARGET_FILE_EXTENSION ".ob"
#define ENTRY_FILE_EXTENSION ".ent"
#define EXTERN_FILE_EXTENSION ".ext"
#define MAX_PATH_LENGTH 128

String *get_files_names(int argc, String *argv);
void verify_files_exists(String *files);
String get_file_name_with_extension(String file_name, String extension);
String generate_dist_path_for_file(String file_path, String extension);

#endif
