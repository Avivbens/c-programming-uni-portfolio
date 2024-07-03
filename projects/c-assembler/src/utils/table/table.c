#include "table.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Hash function to generate a hash value for a given key
 */
static unsigned int hash(String key) {
    unsigned long int value = 0;
    unsigned int i = 0;
    unsigned int key_len = strlen(key);

    for (; i < key_len; ++i) {
        value = value * 37 + key[i];
    }
    return value % TABLE_SIZE;
}

HashTable *create_table(void) {
    int i;
    HashTable *hashtable;

    hashtable = (HashTable *)malloc(sizeof(HashTable));
    hashtable->table = (HashNode **)malloc(sizeof(HashNode *) * TABLE_SIZE);

    if (hashtable == NULL || hashtable->table == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for hashtable\n");
        exit(EXIT_FAILURE);
    }

    /**
     * Initialize all the table elements to NULL
     */
    for (i = 0; i < TABLE_SIZE; i++) {
        hashtable->table[i] = NULL;
    }

    return hashtable;
}

void insert_table(HashTable *hashtable, String key, String value) {
    unsigned int slot = hash(key);
    HashNode *new_node = (HashNode *)malloc(sizeof(HashNode));

    if (new_node == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for new node\n");
        exit(EXIT_FAILURE);
    }

    new_node->key = strdup(key);
    new_node->value = strdup(value);
    new_node->next = hashtable->table[slot];
    hashtable->table[slot] = new_node;
}

String get_table(HashTable *hashtable, String key) {
    unsigned int slot = hash(key);
    HashNode *node = hashtable->table[slot];

    while (node != NULL && strcmp(node->key, key) != 0) {
        node = node->next;
    }

    if (node == NULL) {
        return NULL;
    }

    return node->value;
}

int has_table(HashTable *hashtable, String key) {
    return get_table(hashtable, key) != NULL;
}

int remove_table(HashTable *hashtable, String key) {
    unsigned int slot = hash(key);
    HashNode *node = hashtable->table[slot];
    HashNode *prev = NULL;

    while (node != NULL && strcmp(node->key, key) != 0) {
        prev = node;
        node = node->next;
    }

    /**
     * The key was not found in the hashtable
     */
    if (node == NULL) {
        return REMOVE_SUCCESS;
    }

    /**
     * The key was found in the hashtable
     */
    if (prev == NULL) {
        hashtable->table[slot] = node->next;
    } else {
        prev->next = node->next;
    }

    free(node->key);
    free(node->value);
    free(node);

    return REMOVE_FAIL;
}

void print_table(HashTable *hashtable) {
    int i;

    for (i = 0; i < TABLE_SIZE; i++) {
        HashNode *node = hashtable->table[i];
        while (node != NULL) {
            printf("\n-------------\n");
            printf("Key: \n%s\n\nValue: \n%s\n", node->key, node->value);
            printf("\n-------------\n");
            node = node->next;
        }
    }
}

void free_hashtable(HashTable *hashtable) {
    int i;

    for (i = 0; i < TABLE_SIZE; i++) {
        HashNode *node = hashtable->table[i];
        while (node != NULL) {
            HashNode *temp = node;
            node = node->next;
            free(temp->key);
            free(temp->value);
            free(temp);
        }
    }

    free(hashtable->table);
    free(hashtable);
}
