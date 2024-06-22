#include <stdio.h>
#include <stdlib.h>

#include "./cli/cli-parser.h"
#include "./file-handler/file-handler.h"
#include "./utils/string.h"

int main(int argc, char *argv[]) {
    int i = 0;

    /**
     * Get the files names from the command line arguments
     */
    String *files = get_files_names(argc, argv);

    /**
     * Verify all files exists
     */
    for (i = 0; files[i] != NULL; i++) {
        if (!is_file_exists(files[i])) {
            fprintf(stderr, "Error: File %s does not exist\n", files[i]);
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; files[i] != NULL; i++) {
        printf("%s\n", files[i]);
    }

    free(files);
    return 0;
}
