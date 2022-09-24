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
static ll_node * ll_get_node(ll_list *list, uintmax_t i);
static void ll_print(ll_list *list, void (*func_print)(DATA_TYPE data), void (*func_clean)(ll_list *list));

/* ********************* 'main' function defintion SECTION (UNIT-TEST) ********************** */

#ifdef _MAIN_LINKED_LIST_

#define LEN(ARR) (*(&ARR+1)-ARR)

void func_clean(ll_list *list);
void func_print(int data);

int main() {
    ll_list *list = ll_create();

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
        ll_delete(list, to_delete[i]);
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

    ll_destroy(list);

    return 0;
}

void func_clean(ll_list *list) {
    printf("\b\b \n");
}

void func_print(int data) {
    printf("%d, ", data);
}

#endif

/* ********************* function definition(s) SECTION ********************** */

ll_list * ll_create() {
    ll_list *new_list = (ll_list *) malloc(sizeof(ll_list));
    new_list->length = 0, new_list->head = NULL, new_list->tail = NULL;   
    return new_list;
}

static ll_node * ll_create_node(DATA_TYPE data) {
    ll_node *new_node = (ll_node *) malloc(sizeof(ll_node));
    new_node->data = data, new_node->next = NULL;
    return new_node;
}

static ll_node * ll_get_node(ll_list *list, uintmax_t i) {
    ll_node *node = list->head;
    for (uintmax_t j = 0; j < i; j++) {
        node = node->next;
    }
    return node;
}

DATA_TYPE ll_get(ll_list *list, uintmax_t i) {
    if (i < list->length) {
        return ll_get_node(list, i)->data;
    }
    return DEFAULT_VALUE;
}

void ll_replace(ll_list *list, DATA_TYPE data, uintmax_t i) {
    if (i < list->length) {
        ll_get_node(list, i)->data = data;
    }
}

void ll_insert(ll_list *list, DATA_TYPE data, uintmax_t i) {
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
            ll_node *previous_node = list->head;
            for (uintmax_t j = 1; j < i; j++) {
                previous_node = previous_node->next;
            }
            new_node->next = previous_node->next;
            previous_node->next = new_node;
        }
        list->length++;
    }
}

void ll_delete(ll_list *list, uintmax_t i) {
    if (i < list->length) {
        if (list->length == 1) {                            /* Case: List with one element only. */
            free(list->head);
            list->head = NULL;
            list->tail = NULL;
        } else if (i == 0) {                                /* Case: Deleting at start of index. */
            ll_node *node_to_delete = list->head;
            list->head = list->head->next;
            free(node_to_delete);
        } else {                                            /* Case: Deleting anywhere else. */
            ll_node *previous_node = list->head;
            for (uintmax_t j = 1; j < i; j++) {
                previous_node = previous_node->next;
            }
            ll_node *node_to_delete = previous_node->next;
            previous_node->next = node_to_delete->next;
            free(node_to_delete);                               /* Sub-case: Deleting at end of list. */
            if (i == list->length-1) {
                list->tail = previous_node;
            }
        }
        list->length--;
    }
}

void ll_append(ll_list *list, DATA_TYPE data) {
    ll_node *new_node = ll_create_node(data);
    if (list->tail != NULL) {
        list->tail = list->tail->next = new_node;     /* Case: List not empty. */
    } else {
        list->tail = list->head = new_node;           /* Case: List empty. */
    }
    list->length++;
}

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

void ll_destroy(ll_list *list) {
    ll_node *node = list->head, *next_node;
    if (node != NULL) {
        next_node = node->next;
        free(node);
        node = next_node;
    }
    free(list);
}

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
