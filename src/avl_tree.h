/**
 ****************************************************************
 * @file            : avl_tree.h
 * @author          : Eng. Hazem Mostafa Abdelaziz Anwer
 * @brief           : Declarations, macros, and structs of the implementation of an AVL Binary Search Tree (BST).
 * **************************************************************
 **/

#ifndef _AVL_TREE_H_
#define _AVL_TREE_H_

/* ********************* #include SECTION ********************** */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "shared_defs.h"

/* ********************* struct(s) SECTION ********************** */

/**
 *  @brief      : Node structure (where items are stored).
**/
typedef struct AVL_NODE {
    struct AVL_NODE *lchild;
    struct AVL_NODE *rchild;
    DATA_TYPE data;
    signed char balance;
} avl_node;

/**
 *  @brief      : Tree structure.
**/
typedef struct AVL_TREE {
    avl_node *root;
    LENGTH_DT length;
} avl_tree;

/* ********************* #include SECTION (2) ********************** */

#include "linked_list.h"                            /* This section is for #include's that must follow the struct definitions */

/* ********************* function declaration(S) SECTION ********************** */

/**
 *  @brief      : Create and intialize an AVL tree, and return a pointer to it.
 *  @param      : None.
 *  @return     : Pointer to tree.
**/
avl_tree * avl_create();

/**
 *  @brief      : Get data stored at an index. If index out of bounds, returns DEFAULT_VALUE stored in 'shared_defs.h'.
 *  @param      : [ Tree. ]
 *                [ Index. ]
 *  @return     : Data stored.
**/
DATA_TYPE avl_get(avl_tree *tree, LENGTH_DT i);

/**
 *  @brief      : Insert data in a tree, and ignore balances (unbalanced BST insertion).
 *  @param      : [ Tree. ]
 *                [ Data to insert. ]
 *                [ Function that receives the new data and the data of the current traverse node, and returns 0 (right) or 1 (left). ]
 *  @return     : None.
**/
void avl_insert_unbalanced(avl_tree *tree, DATA_TYPE data, unsigned char (*f_compare)(DATA_TYPE new_data, DATA_TYPE old_data));

/**
 *  @brief      : Insert data in a tree, and balance (AVL BST insertion).
 *  @param      : [ Tree. ]
 *                [ Data to insert. ]
 *                [ Function that receives the new data and the data of the current traverse node, and returns 0 (right) or 1 (left). ]
 *  @return     : None.
**/
void avl_insert(avl_tree *tree, DATA_TYPE data, unsigned char (*f_compare)(DATA_TYPE new_data, DATA_TYPE old_data));

/**
 *  @brief      : Index to delete item from. If index out of bounds, nothing happens. Ignores balances (unbalanced BST deletion).
 *  @param      : [ Tree. ]
 *                [ Index to delete at. ]
 *                [ Function that receives the new data and the data of the current traverse node, and returns 0 (right) or 1 (left). ]
 *  @return     : None.
**/
DATA_TYPE avl_delete_unbalanced(avl_tree *tree, LENGTH_DT i, unsigned char (*f_compare)(DATA_TYPE new_data, DATA_TYPE old_data));

/**
 *  @brief      : Index to delete item from. If index out of bounds, nothing happens. Balances (AVL BST deletion).
 *  @param      : [ Tree. ]
 *                [ Index to delete at. ]
 *                [ Function that receives the new data and the data of the current traverse node, and returns 0 (right) or 1 (left). ]
 *  @return     : None.
**/
DATA_TYPE avl_delete(avl_tree *tree, LENGTH_DT i, unsigned char (*f_compare)(DATA_TYPE new_data, DATA_TYPE old_data));

/**
 *  @brief      : Get the height of a tree.
 *  @param      : [ Tree. ]
 *  @return     : Height of tree.
**/
LENGTH_DT avl_height(avl_tree *tree);

/**
 *  @brief      : Returns a list from a tree (left-to-right). Tree is unmodified.
 *  @param      : [ Tree. ]
 *  @return     : Pointer to list.
**/
ll_list * avl_make_list(avl_tree *tree);

/**
 *  @brief      : Deletes all items in a tree (resets a tree).
 *  @param      : [ Tree. ]
 *  @return     : None.
**/
void avl_delete_all(avl_tree *tree);

/**
 *  @brief      : De-allocates a tree and all its items.
 *  @param      : [ Tree. ]
 *  @return     : None.
**/
void avl_destroy(avl_tree *tree);

/**
 *  @brief      : Prints a tree, level-by-level.
 *  @param      : [ Tree. ]
 *                [ Function that is passed the data at each node. ]
 *                [ Unit size (no. of chars) of each 'f_print' call. ]
 *  @return     : None.
**/
void avl_print(avl_tree *tree, void (*f_print)(DATA_TYPE data), unsigned char unit_size);

#endif