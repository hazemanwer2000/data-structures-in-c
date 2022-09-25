/**
 ****************************************************************
 * @file			: linked_list.c
 * @author 			: Eng. Hazem Mostafa Abdelaziz Anwer
 * @brief 			: Implementation of a linked list, with stack and queue functions.
 * **************************************************************
 **/

/* ********************* #include SECTION ********************** */

#include "linked_list.h"

/* ********************* static function declaration(s) SECTION ********************** */

static ll_node * ll_create_node(DATA_TYPE data);
static ll_node * ll_get_node(ll_list *list, LENGTH_DT i);
static ll_node * ll_get_previous_node(ll_list *list, LENGTH_DT i);
static void ll_print(ll_list *list, void (*func_print)(DATA_TYPE data), void (*func_clean)(ll_list *list));

/* ********************* function definition(s) SECTION ********************** */

/**
 *  @brief      : Allocating dynamic memory for a list structure, initializing and returning the pointer.
 *  @param      : None.
 *  @return     : Pointer to the dynamically allocated list.
**/
ll_list * ll_create() {
    ll_list *new_list = (ll_list *) malloc(sizeof(ll_list));
    new_list->length = 0, new_list->head = NULL, new_list->tail = NULL;   
    return new_list;
}

/**
 *  @brief      : (For internal use) Allocating dynamic memory for a list node, initializing and returning the pointer.
 *  @param      : Data to store.
 *  @return     : Pointer to the dynamically allocated node.
**/
static ll_node * ll_create_node(DATA_TYPE data) {
    ll_node *new_node = (ll_node *) malloc(sizeof(ll_node));
    new_node->data = data, new_node->next = NULL;
    return new_node;
}

/**
 *  @brief      : (For internal use) Get the node at an index in the list. 
 *                  Does not perform any checking on the index. Assumes it is correct.
 *  @param      : List to search in, index to use.
 *  @return     : Pointer to the node.
**/
static ll_node * ll_get_node(ll_list *list, LENGTH_DT i) {
    ll_node *node = list->head;
    for (LENGTH_DT j = 0; j < i; j++) {
        node = node->next;
    }
    return node;
}

/**
 *  @brief      : (For internal use) Get the previous node at an index in the list. 
 *                  Does not perform any checking on the index. Assumes it is correct.
 *  @param      : List to search in, index to use.
 *  @return     : Pointer to the node.
**/
static ll_node * ll_get_previous_node(ll_list *list, LENGTH_DT i) {
    ll_node *node = list->head;
    for (LENGTH_DT j = 1; j < i; j++) {
        node = node->next;
    }
    return node;
}

/**
 *  @brief      : Get the value at an index in the list. If fails, because index is out of bounds,
 *                  then, return a default value, set in the header file.
 *  @param      : List to search in, index to use.
 *  @return     : Stored data.
**/
DATA_TYPE ll_get(ll_list *list, LENGTH_DT i) {
    if (i < list->length) {
        return ll_get_node(list, i)->data;
    }
    return DEFAULT_VALUE;
}

/**
 *  @brief      : Replace value at an index in the list.
 *  @param      : List to search in, index to use.
 *  @return     : None.
**/
void ll_replace(ll_list *list, DATA_TYPE data, LENGTH_DT i) {
    if (i < list->length) {
        ll_get_node(list, i)->data = data;
    }
}

/**
 *  @brief      : Inserting value at a specific index. It will occupy that index, and shift any other nodes to the right.
 *                  Four cases are attended to: Empty list, Insertion at beginning (append), Insertion at end (prepend),
 *                  and any other case (must search for previous node). Before anything, must check if index already exists,
 *                  (or appending). If index does not match these conditions, then insertion does not happen.
 *  @param      : List to work with, data to insert, index to insert at.
 *  @return     : None.
**/
void ll_insert(ll_list *list, DATA_TYPE data, LENGTH_DT i) {
    if (i <= list->length) {
        ll_node *new_node = ll_create_node(data);
        if (list->head == NULL) {                           /* Case: List empty. */
            list->head = list->tail = new_node;
        } else if (i == list->length) {                     /* Case: Inserting at end of list. */
            list->tail = list->tail->next = new_node;
        } else if (i == 0) {                                /* Case: Inserting at beginning of list. */
            new_node->next = list->head;
            list->head = new_node;
        } else {                                            /* Case: Inserting anywhere else. */
            ll_node *previous_node = ll_get_previous_node(list, i);
            new_node->next = previous_node->next;
            previous_node->next = new_node;
        }
        list->length++;
    }
}

/**
 *  @brief      : Deleting a node at a specific index. Index must be already existing, otherwise deletion is not done.
 *                  Index checking handles the case where the list's length is zero. Three cases remain, then:
 *                  Deleting from single-element list, deleting at the start of a list, and deleting anywhere else.
 *                  A sub-case of deleting at the end of a list is handled, where the tail needs to change.
 *                  (Note: You cannot delete a tail directly, since this is a singly linked list.)
 *                  The data of the node to-be-deleted is returned for convenience. If no deletion occurs,
 *                  A default value is returned, set in the header file.
 *  @param      : List to delete from, index to work with.
 *  @return     : Stored data.
**/
DATA_TYPE ll_delete(ll_list *list, LENGTH_DT i) {
    if (i < list->length) {
        DATA_TYPE data;
        if (list->length == 1) {                            /* Case: List with one element only. */
            data = list->head->data;
            free(list->head);
            list->head = NULL;
            list->tail = NULL;
        } else if (i == 0) {                                /* Case: Deleting at start of list. */
            ll_node *node_to_delete = list->head;
            list->head = list->head->next;
            data = node_to_delete->data;
            free(node_to_delete);
        } else {                                            /* Case: Deleting anywhere else. */
            ll_node *previous_node = ll_get_previous_node(list, i);
            ll_node *node_to_delete = previous_node->next;
            previous_node->next = node_to_delete->next;
            data = node_to_delete->data;
            free(node_to_delete);                               /* Sub-case: Deleting at end of list. */
            if (i == list->length-1) {
                list->tail = previous_node;
            }
        }
        list->length--;
        return data;
    }
    return DEFAULT_VALUE;
}

/**
 *  @brief      : Append to a list. Must handle case where list is empty.
 *  @param      : List, data.
 *  @return     : None.
**/
void ll_append(ll_list *list, DATA_TYPE data) {
    ll_node *new_node = ll_create_node(data);
    if (list->tail != NULL) {
        list->tail = list->tail->next = new_node;     /* Case: List not empty. */
    } else {
        list->tail = list->head = new_node;           /* Case: List empty. */
    }
    list->length++;
}

/**
 *  @brief      : Prepend to a list. Must handle case where list is empty.
 *  @param      : List, data.
 *  @return     : None.
**/
void ll_prepend(ll_list *list, DATA_TYPE data) {
    ll_node *new_node = ll_create_node(data);
    if (list->head != NULL) {
        new_node->next = list->head;                  /* Case: List not empty. */
        list->head = new_node;
    } else {
        list->head = list->tail = new_node;           /* Case: List empty. */
    }
    list->length++;
}

/**
 *  @brief      : Delete and deallocate (free) each node in a list, without deallocating the list itself.
 *  @param      : List to deallocate items of.
 *  @return     : None.
**/
void ll_delete_all(ll_list *list) {
    
}

/**
 *  @brief      : Deallocate (free) each node in a list, then deallocate the list itself. Pointer to list
 *                  should not be used thereafter, because it points to already deallocated memory.
 *  @param      : List to deallocate.
 *  @return     : None.
**/
void ll_destroy(ll_list *list) {
    ll_node *node = list->head, *next_node;
    if (node != NULL) {
        next_node = node->next;
        free(node);
        node = next_node;
    }
    free(list);
}

/**
 *  @brief      : (internal use only) Print a list. Must pass a two function pointers,
 *                  one is used to print each node (passed the data, not the node), and another
 *                  is called at the end (passed a reference to the list) for cleaning, and perhaps,
 *                  printing length of list, etc. 
 *  @param      : list, function pointer to a node-print function, function pointer to a post-print cleaning function
 *  @return     : None.
**/
static void ll_print(ll_list *list, void (*func_print)(DATA_TYPE data), void (*func_clean)(ll_list *list)) {
    if (list->length != 0) {
        ll_node *node = list->head;
        while (node != NULL) {
            func_print(node->data);
            node = node->next;
        }
        func_clean(list);
    }
}

/* ********************* 'main' function defintion SECTION (UNIT-TEST) ********************** */

#ifdef _MAIN_LINKED_LIST_

#define LEN(ARR) (*(&ARR+1)-ARR)

void func_clean(ll_list *list);
void func_print(int data);
void testing_list(ll_list *list);
void testing_stack(ll_list *list);
void testing_queue(ll_list *list);

int main() {
    ll_list *list = ll_create();

    /* testing_list(list); */
    /* testing_stack(list); */
    /* testing_queue(list); */

    ll_destroy(list);

    return 0;
}

void testing_queue(ll_list *list) {
    int to_enqueue[] = {1, 2, 3, 4};
    for (int i = 0; i < LEN(to_enqueue); i++) {
        printf("Enqueuing: %d\n", to_enqueue[i]);
        ll_enqueue(list, to_enqueue[i]);
        ll_print(list, func_print, func_clean);
    }

    putchar('\n');

    for (int i = 0; i < 5; i++) {
        printf("Dequeuing: %d\n", ll_dequeue(list));
    }

    putchar('\n');

    int to_enqueue2[] = {5, 6, 7};
    for (int i = 0; i < LEN(to_enqueue2); i++) {
        printf("Enqueuing: %d\n", to_enqueue2[i]);
        ll_enqueue(list, to_enqueue2[i]);
        ll_print(list, func_print, func_clean);
    }

    printf("\nLength: %lu\n", list->length);
}

void testing_stack(ll_list *list) {
    int to_push[] = {1, 2, 3, 4};
    for (int i = 0; i < LEN(to_push); i++) {
        printf("Pushing: %d\n", to_push[i]);
        ll_push(list, to_push[i]);
        ll_print(list, func_print, func_clean);
    }

    putchar('\n');

    for (int i = 0; i < 5; i++) {
        printf("Popping: %d\n", ll_pop(list));
    }

    putchar('\n');

    int to_push2[] = {5, 6, 7};
    for (int i = 0; i < LEN(to_push2); i++) {
        printf("Pushing: %d\n", to_push2[i]);
        ll_push(list, to_push2[i]);
        ll_print(list, func_print, func_clean);
    }

    printf("\nLength: %lu\n", list->length);
}

void testing_list(ll_list* list) {
    int to_append[] = {1, 2};
    for (int i = 0; i < LEN(to_append); i++) {
        printf("Appending: %d\n", to_append[i]);
        ll_append(list, to_append[i]);
        ll_print(list, func_print, func_clean);
    }

    putchar('\n');

    int to_prepend[] = {3, 4};
    for (int i = 0; i < LEN(to_prepend); i++) {
        printf("Prepending: %d\n", to_prepend[i]);
        ll_prepend(list, to_prepend[i]);
        ll_print(list, func_print, func_clean);
    }

    putchar('\n');

    int to_insert[] = {0, 5, 5, 8, 3};
    int to_insert_data[] = {0, 9, 8, 10, 13};
    for (int i = 0; i < LEN(to_insert); i++) {
        printf("Inserting: %d, Data: %d\n", to_insert[i], to_insert_data[i]);
        ll_insert(list, to_insert_data[i], to_insert[i]);
        ll_print(list, func_print, func_clean);
    }

    putchar('\n');

    int to_delete[] = {0, 5, 2, 5, 4};
    for (int i = 0; i < LEN(to_delete); i++) {
        printf("Deleting: %d\n", to_delete[i]);
        printf("Item deleted: %d\n", ll_delete(list, to_delete[i]));
        ll_print(list, func_print, func_clean);
    }

    putchar('\n');

    int to_replace[] = {0, 1, 2, 3, 4};
    int to_replace_data[] = {0, 1, 2, 3, 4};
    for (int i = 0; i < LEN(to_replace); i++) {
        printf("Replacing: %d, Data: %d\n", to_replace[i], to_replace_data[i]);
        ll_replace(list, to_replace_data[i], to_replace[i]);
        ll_print(list, func_print, func_clean);
    }

    putchar('\n');

    int to_get[] = {4, 3, 2, 1, 0};
    for (int i = 0; i < LEN(to_get); i++) {
        printf("Getting: %d\n", to_get[i]);
        printf("Result-> %15d\n", ll_get(list, to_get[i]));
    }

    printf("\nLength: %lu\n", list->length);
}

void func_clean(ll_list *list) {
    printf("\b\b \n");
}

void func_print(int data) {
    printf("%d, ", data);
}

#endif