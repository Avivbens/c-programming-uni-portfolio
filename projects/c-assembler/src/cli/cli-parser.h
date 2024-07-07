#include "../file-handler/file-handler.h"
#include "../utils/string/string.h"

#define ORIGINAL_FILE_EXTENSION ".as"
#define POST_PROCESS_FILE_EXTENSION ".am"
#define MAX_PATH_LENGTH 128

String *get_files_names(int argc, String *argv);
