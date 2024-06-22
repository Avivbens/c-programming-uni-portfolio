#include <stdio.h>
#include <stdlib.h>

#include "./cli/cli-parser.h"

int main(int argc, char *argv[]) {
    int i = 0;

    String *files = get_files_names(argc, argv);

    for (i = 0; files[i] != NULL; i++) {
        printf("%s\n", files[i]);
    }

    free(files);
    return 0;
}
