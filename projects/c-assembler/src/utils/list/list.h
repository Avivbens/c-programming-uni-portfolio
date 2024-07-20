#include <stdint.h>

#include "../string/string.h"

#ifndef SET_H
#define SET_H

#define TABLE_SIZE 100

#define REMOVE_SUCCESS 1
#define REMOVE_FAIL 0

typedef struct ListNode {
    String key;
    String value;
    struct ListNode *next;
} ListNode;

typedef struct LinkedList {
    ListNode **list;
} LinkedList;
#endif

LinkedList *create_list(void);

void insert_list(LinkedList *hashmap, String key, String value);
String get_list(LinkedList *hashmap, String key);
int has_list(LinkedList *hashmap, String key);
int remove_list(LinkedList *hashmap, String key);
void free_linkedList(LinkedList *hashmap);
void print_list(LinkedList *linkedList);
