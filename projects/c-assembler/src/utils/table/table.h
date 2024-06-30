#include <stdint.h>

#include "../string.h"

#ifndef SET_H
#define SET_H

#define TABLE_SIZE 100

#define REMOVE_SUCCESS 1
#define REMOVE_FAIL 0

typedef struct HashNode {
    String key;
    String value;
    struct HashNode *next;
} HashNode;

typedef struct HashTable {
    HashNode **table;
} HashTable;
#endif

HashTable *create_table(void);

void insert_table(HashTable *hashmap, String key, String value);
String get_table(HashTable *hashmap, String key);
int has_table(HashTable *hashmap, String key);
int remove_table(HashTable *hashmap, String key);
void free_hashtable(HashTable *hashmap);
