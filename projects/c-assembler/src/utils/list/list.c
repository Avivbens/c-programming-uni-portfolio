#include "list.h"

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

LinkedList *create_list(void) {
    int i;
    LinkedList *linkedList;

    linkedList = (LinkedList *)malloc(sizeof(LinkedList));
    linkedList->list = (ListNode **)malloc(sizeof(ListNode *) * TABLE_SIZE);

    if (linkedList == NULL || linkedList->list == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for linkedList\n");
        exit(EXIT_FAILURE);
    }

    /**
     * Initialize all the list elements to NULL
     */
    for (i = 0; i < TABLE_SIZE; i++) {
        linkedList->list[i] = NULL;
    }

    return linkedList;
}

/**
 * Insert a key-value pair into the linkedList
 */
void insert_list(LinkedList *linkedList, String key, void *value,
                 int value_size) {
    String copy_key = strdup(key);
    void *copy_value = memcpy(malloc(value_size), value, value_size);

    unsigned int slot = hash(key);
    ListNode *new_node = (ListNode *)malloc(sizeof(ListNode));

    if (new_node == NULL || copy_key == NULL || copy_value == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for new node\n");
        exit(EXIT_FAILURE);
    }

    new_node->key = copy_key;
    new_node->value = copy_value;
    new_node->next = linkedList->list[slot];
    linkedList->list[slot] = new_node;
}

void *get_list(LinkedList *linkedList, String key) {
    unsigned int slot = hash(key);
    ListNode **list = linkedList->list;
    ListNode *node;

    if (list == NULL) {
        return NULL;
    }

    node = list[slot];

    if (node == NULL) {
        return NULL;
    }

    while (node != NULL && strcmp(node->key, key) != 0) {
        node = node->next;
    }

    return node->value;
}

int has_list(LinkedList *linkedList, String key) {
    return get_list(linkedList, key) != NULL;
}

int remove_list(LinkedList *linkedList, String key) {
    unsigned int slot = hash(key);
    ListNode **list = linkedList->list;
    ListNode *prev = NULL;
    ListNode *node;

    if (list == NULL) {
        return REMOVE_FAIL;
    }

    node = list[slot];

    /**
     * The key was not found in the linkedList
     */
    if (node == NULL) {
        return REMOVE_FAIL;
    }

    while (node != NULL && strcmp(node->key, key) != 0) {
        prev = node;
        node = node->next;
    }

    /**
     * The key was found in the linkedList
     */
    if (prev == NULL) {
        list[slot] = node->next;
    } else {
        prev->next = node->next;
    }

    free(node->key);
    free(node->value);
    free(node);

    return REMOVE_SUCCESS;
}

void print_list(LinkedList *linkedList, ValuePrinter printer) {
    ListNode **list = linkedList->list;
    ListNode *node;
    int i;

    if (list == NULL) {
        return;
    }

    for (i = 0; i < TABLE_SIZE; i++) {
        node = list[i];
        while (node != NULL) {
            printf("\n-------------\n");
            printf("Key: \n%s\n\nValue: \n", node->key);
            printer(node->value);
            printf("\n\n-------------\n");
            node = node->next;
        }
    }
}

void free_linkedList(LinkedList *linkedList) {
    ListNode **list = linkedList->list;
    ListNode *node;
    ListNode *temp;

    int i;

    for (i = 0; i < TABLE_SIZE; i++) {
        node = list[i];
        while (node != NULL) {
            temp = node;
            node = node->next;
            free(temp->key);
            free(temp->value);
            free(temp);
        }
    }

    free(linkedList->list);
    free(linkedList);
}
