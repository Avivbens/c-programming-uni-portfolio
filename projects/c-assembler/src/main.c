#include <stdio.h>
#include <stdlib.h>

#include "./cli/cli-parser.h"
#include "./converters/output-generator.h"
#include "./parsers/labels-parser/labels-parser.h"
#include "./parsers/macros-parser/macros-parser.h"
#include "./utils/file/file.h"
#include "./utils/string/string.h"

int main(int argc, char *argv[]) {
    /**
     * Get the files names from the command line arguments
     */
    String *files = get_files_names(argc, argv);
    String *post_processed_files;

    /**
     * Verify all files exists
     */
    verify_files_exists(files);

    /**
     * Handle macros - register and create post-processed files
     */
    post_processed_files = handle_macros(files);

    /**
     * Handle labels - register and add address
     */
    handle_labels(post_processed_files);

    /**
     * Create all targeted files
     */
    handle_output(post_processed_files);
    free(files);
    return 0;
}
