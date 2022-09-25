/**
 ****************************************************************
 * @file			: avl_tree.h
 * @author 			: Eng. Hazem Mostafa Abdelaziz Anwer
 * @brief 			: Declarations, macros, and structs of the implementation of an AVL Binary Search Tree (BST).
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
    unsigned long long length;
} avl_tree;

/* ********************* #include SECTION (2) ********************** */

#include "linked_list.h"

/* ********************* function declaration(S) SECTION ********************** */

/**
 *  @brief      : Create and intialize an AVL tree, and return a pointer to it.
 *  @param      : None.
 *  @return     : Pointer to tree.
**/
avl_tree * avl_create();

/**
 *  @brief      : 
 *  @param      : 
 *  @return     : 
**/
void avl_insert_unbalanced(avl_tree *tree, DATA_TYPE data, unsigned char (*f_compare)(DATA_TYPE new_data, DATA_TYPE old_data));

/**
 *  @brief      : 
 *  @param      : 
 *  @return     : 
**/
LENGTH_DT avl_height(avl_tree *tree);

/**
 *  @brief      : 
 *  @param      : 
 *  @return     : 
**/
ll_list * avl_tree_to_list(avl_tree *tree);

/**
 *  @brief      : 
 *  @param      : 
 *  @return     : 
**/
void avl_print(avl_tree *tree, void (*f_print)(DATA_TYPE data), unsigned char unit_size);

#endif