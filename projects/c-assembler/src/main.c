#include <stdio.h>
#include <stdlib.h>

#include "./cli/cli-parser.h"
#include "./data/labels-data.h"
#include "./file-handler/file-handler.h"
#include "./init/init-symbols.c"
#include "./macros-parser/macros-parser.h"
#include "./utils/string/string.h"

int main(int argc, char *argv[]) {
    ic = 100;
    /**
     * Get the files names from the command line arguments
     */
    String *files = get_files_names(argc, argv);

    /**
     * Verify all files exists
     */
    verify_files_exists(files);

    /**
     * Handle macros - register and create post-processed files
     */
    handle_macros(files);
    HashTable *symbolsTable = create_symbols_table_first_iteration(files);
    create_symbols_table_second_iteration(files, symbolsTable);
    free(files);
    return 0;
}
