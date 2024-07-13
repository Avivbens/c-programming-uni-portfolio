#include <stdio.h>
#include <stdlib.h>

#include "../../src/cli/cli-parser.c"
#include "../../src/cli/cli-parser.h"
#include "../constants/constants.h"
#include "../data/labels-data.h"
#include "../utils/label/label.c"
#include "../utils/string/string.c"
#include "../utils/table/table.c"
#include "../utils/table/table.h"

typedef struct Symbol {
    char *name;
    char *value;
} Symbol;

static int label_registration(String file_name, HashTable *symbolsTable) {
    FILE *file;
    int exit_code = EXIT_SUCCESS;
    char line[MAX_LINE_LENGTH];
    String file_path =
        get_file_name_with_extension(file_name, ORIGINAL_FILE_EXTENSION);
    String label_name = (String)malloc(MAX_LABEL_LENGTH);

    if (label_name == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for label name\n");
        exit(EXIT_FAILURE);
    }

    file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Error: Could not open file %s\n", file_name);
        return EXIT_FAILURE;
    }

    while (fgets(line, sizeof(line), file)) {
        String first_word = get_first_from_line(line);
        int is_word_label = is_label(first_word);
        if (is_word_label == 0)
            continue;
        else {
            // check if the word if not already in the symbol table or extern
            // table if yes, exit with error if not - insert
            if (get_table(symbolsTable, first_word) != NULL) {
                printf("label already exits");
                exit(EXIT_FAILURE);
            }
            insert_table(symbolsTable, first_word, "0");  // initial value
        }
    }
}

static int label_fill(String file_name, HashTable *symbolsTable) {
    FILE *file;
    int exit_code = EXIT_SUCCESS;
    char line[MAX_LINE_LENGTH];
    String file_path =
        get_file_name_with_extension(file_name, ORIGINAL_FILE_EXTENSION);
    String label_name = (String)malloc(MAX_LABEL_LENGTH);
    if (label_name == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for label name\n");
        exit(EXIT_FAILURE);
    }
    file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Error: Could not open file %s\n", file_name);
        return EXIT_FAILURE;
    }
    int word_counter = 0;
    while (fgets(line, sizeof(line), file)) {
        String first_word = get_first_from_line(line);
        int is_word_label = is_label(first_word);
        if (is_word_label == 0) {
            word_counter++;
            continue;
        } else {
            update_table(symbolsTable, first_word,
                         ic + word_counter);  // initial value
        }
    }
}

void create_symbols_table(HashTable *symbolsTable, String *file_names) {
    int i;
    for (i = 0; file_names[i] != NULL; i++) {
        label_registration(file_names[i], symbolsTable);
    }
}

void fill_symbols_table(HashTable *symbolsTable, String *file_names) {
    int i;
    for (i = 0; file_names[i] != NULL; i++) {
        label_fill(file_names[i], symbolsTable);
    }
    ic += 100;
}

HashTable *create_symbols_table_first_iteration(String *file_names) {
    static HashTable *symbolsTable = NULL;

    if (symbolsTable == NULL) {
        symbolsTable = create_table();
        create_symbols_table(symbolsTable, file_names);
    }
    return symbolsTable;
}

void create_symbols_table_second_iteration(String *file_names,
                                           HashTable *symbolsTable) {
    fill_symbols_table(symbolsTable, file_names);
}
