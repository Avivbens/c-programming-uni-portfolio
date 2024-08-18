#include <stdint.h>
#include <stdio.h>

#include "../string/string.h"

#ifndef LIST_H
#define LIST_H

#define TABLE_SIZE 100

#define REMOVE_SUCCESS 1
#define REMOVE_FAIL 0

typedef struct ListNode {
    String key;
    void *value;
    struct ListNode *next;
} ListNode;

typedef struct LinkedList {
    ListNode **list;
} LinkedList;

typedef void (*ValuePrinter)(void *);

LinkedList *create_list(void);

void iterate_list(LinkedList *linkedList,
                  void (*callback)(void *, String, FILE *), String context,
                  FILE *exec);

int get_list_size(LinkedList *linkedList);

void insert_list(LinkedList *linkedList, String key, void *value,
                 int value_size);
void *get_list(LinkedList *linkedList, String key);
int has_list(LinkedList *linkedList, String key);
int remove_list(LinkedList *linkedList, String key);
void print_list(LinkedList *linkedList, ValuePrinter printer);
void free_linkedList(LinkedList *linkedList);

#endif
