/**
 ****************************************************************
 * @file            : linked_list.c
 * @author          : Eng. Hazem Mostafa Abdelaziz Anwer
 * @brief           : Implementation of a linked list, with stack and queue functions.
 * **************************************************************
 **/

/* ********************* #include SECTION ********************** */

#include "linked_list.h"

/* ********************* static function declaration(s) SECTION ********************** */

static ll_node * ll_create_node(DATA_TYPE data);
static ll_node * ll_get_node(ll_list *list, LENGTH_DT i);
static ll_node * ll_get_previous_node(ll_list *list, LENGTH_DT i);
static void ll_deallocate_all(ll_list *list);

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
 *  @param      : [ Data to store. ]
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
 *  @param      : [ List to search in. ]
 *                [ Index to use. ]
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
 *  @param      : [ List to search in. ] 
 *                [ Index to use. ]
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
 *                  (Note: Allows negative indexing if LENGTH_DT is signed.)
 *  @param      : [ List to search in. ]
 *                [ Index to use. ]
 *  @return     : Stored data.
**/
DATA_TYPE ll_get(ll_list *list, LENGTH_DT i) {
    if (i < 0) { i += list->length; }                    /* to allow reverse indexing */

    if (i < list->length) {
        return ll_get_node(list, i)->data;
    }
    return DEFAULT_VALUE;
}

/**
 *  @brief      : Replace value at an index in the list.
 *  @param      : [ List to search in. ]
 *                [ Data to substitute. ]
 *                [ Index to use. ]
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
 *  @param      : [ List to work with. ]
 *                [ Data to insert. ] 
 *                [ Index to insert at. ]
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
 *  @param      : [ List to delete from. ]
 *                [ Index to work with. ]
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
 *  @param      : [ List. ] 
 *                [ Data. ]
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
 *  @param      : [ List. ]
 *                [ Data. ]
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
 *  @brief      : (internal use only) Deallocate (free) each node in a list, without deallocating the list itself,
 *                  or setting the head or tail pointers to NULL, or list length.
 *  @param      : [ List to deallocate items of. ]
 *  @return     : None.
**/
static void ll_deallocate_all(ll_list *list) {
    ll_node *node = list->head, *next_node;
    if (node != NULL) {
        next_node = node->next;
        free(node);
        node = next_node;
    }
}

/**
 *  @brief      : Delete and deallocate (free) each node in a list, without deallocating the list itself.
 *  @param      : [ List to deallocate items of. ]
 *  @return     : None.
**/
void ll_delete_all(ll_list *list) {
    ll_deallocate_all(list);
    list->head = list->tail = NULL, list->length = 0;
}

/**
 *  @brief      : Copy a list into a new list, in the same order, or in reverse.
 *  @param      : [ List to copy. ]
 *                [ Reverse flag (1 to reverse). ]
 *  @return     : None.
**/
ll_list * ll_copy(ll_list *list, unsigned char rev_flag) {
    ll_list * new_list = ll_create();
    ll_node * traverse_node = list->head;
    void (*f_ptr)(ll_list *list, DATA_TYPE data) = rev_flag ? ll_prepend : ll_append;
    while (traverse_node != NULL) {
        f_ptr(new_list, traverse_node->data);
        traverse_node = traverse_node->next;
    }
    return new_list;
}

/**
 *  @brief      : Deallocate (free) each node in a list, then deallocate the list itself. Pointer to list
 *                  should not be used thereafter, because it points to already deallocated memory.
 *  @param      : [ List to deallocate. ]
 *  @return     : None.
**/
void ll_destroy(ll_list *list) {
    ll_deallocate_all(list);
    free(list);
}

/**
 *  @brief      : Print a list. Must pass a two function pointers,
 *                  one is used to print each node (passed the data, not the node), and another
 *                  is called at the end (passed a reference to the list) for cleaning, and perhaps,
 *                  printing length of list, etc. 
 *  @param      : [ List to print. ]
 *                [ Function to be called at each item (passed each item consecutively). ] 
 *                [ Function to be called after all items have been printed (passed list and used for clean-up). ]
 *  @return     : None.
**/
void ll_print(ll_list *list, void (*f_print)(DATA_TYPE data), void (*f_clean)(ll_list *list)) {
    if (list->length != 0) {
        ll_node *node = list->head;
        while (node != NULL) {
            f_print(node->data);
            node = node->next;
        }
        f_clean(list);
    }
}

/* ********************* 'main' function defintion SECTION (UNIT-TEST) ********************** */

#ifdef _MAIN_LINKED_LIST_               /* compile-time switch */

#define LEN(ARR) (*(&ARR+1)-ARR)

void t_insert();
void t_replace();
void t_get();
void t_delete();
void t_append_prepend();
void t_delete_all();
void t_copy();

void print(void *data);

void f_clean(ll_list *list);
void f_print(void *data);

int main() {
    t_insert();
    t_replace();
    t_get();
    t_delete();
    t_append_prepend();
    t_delete_all();
    t_copy();
    return 0;
}

void t_delete() {
    printf("*************** TEST (DELETE) ***************\n");
    ll_list *list = ll_create();
    int arr_data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    for (int i = 0; i < LEN(arr_data); i++) {
        ll_insert(list, arr_data+i, i);
    }
    ll_print(list, f_print, f_clean);
    int arr_index[] = {0, 5, 2, 3, 4, 9};
    for (int i = 0; i < LEN(arr_index); i++) {
        printf("Deleting (i=%d)\n", arr_index[i]);
        print(ll_delete(list, arr_index[i]));
        ll_print(list, f_print, f_clean);
    }
    ll_destroy(list);
}

void t_replace() {
    printf("*************** TEST (REPLACE) ***************\n");
    ll_list *list = ll_create();
    int zero = 0;
    for (int i = 0; i < 6; i++) {
        ll_insert(list, &zero, i);
    }
    ll_print(list, f_print, f_clean);
    int arr_data[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int arr_index[] = {0, 0, 1, 2, 3, 2, 7, 5};
    for (int i = 0; i < LEN(arr_data); i++) {
        printf("Replacing: %d at (i=%d)\n", arr_data[i], arr_index[i]);
        ll_replace(list, arr_data+i, arr_index[i]);
        ll_print(list, f_print, f_clean);
    }
    ll_destroy(list);
}

void t_insert() {
    printf("*************** TEST (INSERT) ***************\n");
    ll_list *list = ll_create();
    int arr_data[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int arr_index[] = {0, 0, 1, 2, 3, 2, 7, 6};
    for (int i = 0; i < LEN(arr_data); i++) {
        printf("Inserting: %d at (i=%d)\n", arr_data[i], arr_index[i]);
        ll_insert(list, arr_data+i, arr_index[i]);
        ll_print(list, f_print, f_clean);
    }
    ll_destroy(list);
}

void t_get() {
    printf("*************** TEST (GET) ***************\n");
    ll_list *list = ll_create();
    int arr_data[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < LEN(arr_data); i++) {
        ll_insert(list, arr_data+i, i);
    }
    ll_print(list, f_print, f_clean);
    for (int i = 0; i < LEN(arr_data); i++) {
        printf("Getting (i=%d)\n", i);
        print(ll_get(list, i));
    }
    ll_destroy(list);
}

void t_append_prepend() {
    printf("*************** TEST (APPEND/PREPEND) ***************\n");
    ll_list *list = ll_create();
    int arr_data[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int len = LEN(arr_data);
    for (int i = 0; i < len/2; i++) {
        printf("Appending %d\n", arr_data[i]);
        ll_append(list, arr_data+i);
        ll_print(list, f_print, f_clean);
    }
    for (int i = len/2; i < len; i++) {
        printf("Prepending %d\n", arr_data[i]);
        ll_prepend(list, arr_data+i);
        ll_print(list, f_print, f_clean);
    }
    ll_destroy(list);
}

void t_delete_all() {
    printf("*************** TEST (DELETE-ALL) ***************\n");
    ll_list *list = ll_create();
    int arr_data[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < LEN(arr_data); i++) {
        ll_insert(list, arr_data+i, i);
    }
    ll_print(list, f_print, f_clean);
    printf("Deleting all...\n");
    ll_delete_all(list);
    for (int i = 0; i < 3; i++) {
        printf("Inserting: %d at (i=%d)\n", arr_data[i], i);
        ll_insert(list, arr_data+i, i);
        ll_print(list, f_print, f_clean);
    }
    ll_destroy(list);
}

void t_copy() {
    printf("*************** TEST (COPY) ***************\n");
    ll_list *list = ll_create();
    int arr_data[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < LEN(arr_data); i++) {
        ll_insert(list, arr_data+i, i);
    }
    ll_print(list, f_print, f_clean);
    printf("Copying...\n");
    ll_list *copy = ll_copy(list, 0);
    ll_print(copy, f_print, f_clean);
    printf("Reversing...\n");
    ll_list *rev = ll_copy(list, 1);
    ll_print(rev, f_print, f_clean);
    printf("Original:\n");
    ll_print(list, f_print, f_clean);
    ll_destroy(list);
    ll_destroy(copy);
    ll_destroy(rev);
}

void print(void *data) {
    printf("%d\n", *((int *) data));
}

void f_clean(ll_list *list) {
    printf("\b\b \n");
}

void f_print(void *data) {
    printf("%d, ", *((int *) data));
}

#endif