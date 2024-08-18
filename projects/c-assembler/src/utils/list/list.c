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

/**
 * Get the size of the linkedList
 * @returns The size of the linkedList
 *
 * @note The size of the linkedList is the number of key-value pairs in the
 * linkedList
 */
int get_list_size(LinkedList *linkedList) {
    ListNode **list = linkedList->list;
    ListNode *node;
    int i;
    int size = 0;

    if (list == NULL) {
        return 0;
    }

    for (i = 0; i < TABLE_SIZE; i++) {
        node = list[i];
        while (node != NULL) {
            size++;
            node = node->next;
        }
    }

    return size;
}

/**
 * Iterate over the linkedList
 *
 * @param linkedList The linkedList to iterate over
 * @param callback The function to call for each value in the linkedList
 *
 * @returns void
 */
void iterate_list(LinkedList *linkedList,
                  void (*callback)(void *, String, FILE *), String context,
                  FILE *exec) {
    ListNode **list = linkedList->list;
    ListNode *node;
    int i;

    if (list == NULL) {
        return;
    }

    for (i = 0; i < TABLE_SIZE; i++) {
        node = list[i];
        while (node != NULL) {
            callback(node->value, context, exec);
            node = node->next;
        }
    }
}

/**
 * Create a new linkedList
 *
 * @returns a pointer to the linkedList
 * @throw NULL if memory allocation fails
 *
 * @note The linkedList is dynamically allocated and must be freed using
 * The value of the linkedList is a dynamic pointer to any type of data
 */
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
 *
 * @param linkedList The linkedList to insert the key-value pair into
 * @param key The key of the pair
 * @param value The value of the pair - a pointer to any type of data
 * @param value_size The size of the value
 *
 * @returns void
 * @throw If memory allocation fails, the program exits with EXIT_FAILURE.
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

/**
 * Get the value of a key from the linkedList
 *
 * @returns The pointer to the value of the key, or NULL if the key is not found
 */
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

/**
 * Check if a key exists in the linkedList
 * @returns 1 if the key exists, 0 otherwise
 */
int has_list(LinkedList *linkedList, String key) {
    return get_list(linkedList, key) != NULL;
}

/**
 * Remove a key from the linkedList
 * @returns REMOVE_SUCCESS if the key was removed, REMOVE_FAIL otherwise
 */
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

/**
 * Print the linkedList
 * @param linkedList The linkedList to print
 * @param printer The function to print the value of the linkedList
 */
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

/**
 * Free the linkedList
 * @param linkedList The linkedList to free
 */
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
