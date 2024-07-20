#include "../utils/file/file.h"
#include "../utils/string/string.h"

#define ORIGINAL_FILE_EXTENSION ".as"
#define POST_PROCESS_FILE_EXTENSION ".am"
#define MAX_PATH_LENGTH 128

String *get_files_names(int argc, String *argv);
void verify_files_exists(String *files);
String get_file_name_with_extension(String file_name, String extension);
