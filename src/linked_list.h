/**
 ****************************************************************
 * @file            : linked_list.h
 * @author          : Eng. Hazem Mostafa Abdelaziz Anwer
 * @brief           : Implementation of a linked list, with stack and queue functions.
 * **************************************************************
 **/

#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

/* ********************* #include SECTION ********************** */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* ********************* #define SECTION ********************** */

#define DATA_TYPE int
#define DEFAULT_VALUE 0

/* ********************* struct(s) SECTION ********************** */

typedef struct LL_NODE {
    struct LL_NODE *next;
    DATA_TYPE data;
} ll_node;

typedef struct LL_LIST {
    ll_node *head;
    ll_node *tail;
    uintmax_t length;
} ll_list;

/* ********************* function declaration(S) SECTION ********************** */

ll_list * ll_create();
DATA_TYPE ll_get(ll_list *list, uintmax_t i);
void ll_replace(ll_list *list, DATA_TYPE data, uintmax_t i);
void ll_insert(ll_list *list, DATA_TYPE data, uintmax_t i);
void ll_delete(ll_list *list, uintmax_t i);
void ll_append(ll_list *list, DATA_TYPE data);
void ll_prepend(ll_list *list, DATA_TYPE data);
void ll_destroy(ll_list *list);

#endif