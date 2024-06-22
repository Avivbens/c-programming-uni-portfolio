#include "./cli-parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

String *get_files_names(int argc, String *argv) {
    String *files = (String *)malloc(argc * sizeof(String));
    int i = 0;

    if (files == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    for (i = 1; i < argc; i++) {
        files[i - 1] = (String)malloc(MAX_PATH_LENGTH * sizeof(char));

        if (files[i - 1] == NULL) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        strcpy(files[i - 1], argv[i]);
        strcat(files[i - 1], FILE_EXTENSION);
    }

    files[argc - 1] = NULL;

    return files;
}
