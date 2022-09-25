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

/**
 *  @brief      : The data-type of choice in the list.
 *                  (Note: Should be 'void *' for generality.)
**/
#define DATA_TYPE void *

/**
 *  @brief      : The default value of the data-type of choice in the list.
 *                  This value is returned by indexable, item-returning functions, when the index is out-of-bounds.
 *                  (Note: Should be '0' for generality.)
**/
#define DEFAULT_VALUE 0

/**
 *  @brief      : The data-type of choice for the length of a list to be stored in.
 *                  (Note: It defines the maximum length of a list.)
**/
#define LENGTH_DT unsigned long

/**
 *  @brief      : Stack functions, implemented as macro functions, aliasing list functions.
**/
#define ll_push(list, data)         ll_prepend(list, data)
#define ll_pop(list)                ll_delete(list, 0)

/**
 *  @brief      : Queue functions, implemented as macro functions, aliasing list functions.
**/
#define ll_enqueue(list, data)      ll_append(list, data)
#define ll_dequeue(list)            ll_delete(list, 0)

/* ********************* struct(s) SECTION ********************** */

/**
 *  @brief      : Node structure (where items are stored).
**/
typedef struct LL_NODE {
    struct LL_NODE *next;
    DATA_TYPE data;
} ll_node;

/**
 *  @brief      : List structure.
**/
typedef struct LL_LIST {
    ll_node *head;
    ll_node *tail;
    LENGTH_DT length;
} ll_list;

/* ********************* function declaration(S) SECTION ********************** */

/**
 *  @brief      : Create a linked list (dynamically, on heap).
 *  @param      : None.
 *  @return     : Pointer to list.
**/
ll_list * ll_create();

/**
 *  @brief      : Get the item at an index in the list. If fails, because index is out of bounds,
 *                  then, return a default value, set in the header file.
 *  @param      : List to search in, index to use.
 *  @return     : Stored data.
**/
DATA_TYPE ll_get(ll_list *list, LENGTH_DT i);

/**
 *  @brief      : Replace item at an index in the list.
 *  @param      : List to search in, index to use.
 *  @return     : None.
**/
void ll_replace(ll_list *list, DATA_TYPE data, LENGTH_DT i);

/**
 *  @brief      : Insert item at an index. Allows appending. If index is out of bounds, nothing happens.
 *  @param      : List to work with, data to insert, index to insert at.
 *  @return     : None.
**/
void ll_insert(ll_list *list, DATA_TYPE data, LENGTH_DT i);

/**
 *  @brief      : Deleting item at index. If index does not exist, nothing happens. Returns item.
 *  @param      : List to delete from, index to work with.
 *  @return     : Stored data.
**/
DATA_TYPE ll_delete(ll_list *list, LENGTH_DT i);

/**
 *  @brief      : Append item to list.
 *  @param      : List to append to, data to append.
 *  @return     : None.
**/
void ll_append(ll_list *list, DATA_TYPE data);

/**
 *  @brief      : Prepend item to list.
 *  @param      : List to prepend to, data to prepend.
 *  @return     : None.
**/
void ll_prepend(ll_list *list, DATA_TYPE data);

/**
 *  @brief      : Destroy list (de-allocated off heap). 
 *                  (Note: If pointers are the data-type, they're de-allocated, and not the data they point to.)
 *  @param      : List to destroy.
 *  @return     : None.
**/
void ll_destroy(ll_list *list);

#endif