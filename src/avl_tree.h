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

/* ********************* #define SECTION ********************** */

#define DATA_TYPE int
#define DEFAULT_VALUE 0

#define LENGTH_DT unsigned long

/* ********************* struct(s) SECTION ********************** */

typedef struct AVL_NODE {
    struct AVL_NODE *lchild;
    struct AVL_NODE *rchild;
    DATA_TYPE data;
    signed char balance;
} avl_node;

typedef struct AVL_TREE {
    avl_node *root;
    unsigned long long length;
} avl_tree;

/* ********************* function declaration(S) SECTION ********************** */

avl_tree * avl_create();
void avl_insert(avl_tree *tree, DATA_TYPE data, unsigned char (*f_compare)(DATA_TYPE new_data, DATA_TYPE old_data));
LENGTH_DT avl_height(avl_tree *tree);

#endif