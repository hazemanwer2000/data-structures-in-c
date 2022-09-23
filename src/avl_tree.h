/**
 ****************************************************************
 * @file			: avl_tree.h
 * @author 			: Eng. Hazem Mostafa Abdelaziz Anwer
 * @brief 			: Declarations, macros, and structs of the implementation of an AVL Binary Search Tree (BST).
 * **************************************************************
 **/

/* ********************* #include SECTION ********************** */

#include <stdio.h>
#include <stdlib.h>

/* ********************* #define SECTION ********************** */

#define DATA_TYPE int

/* ********************* struct(s) SECTION ********************** */

typedef struct AVL_NODE {
    struct avl_node *lchild;
    struct avl_node *rchild;
    DATA_TYPE data;
    signed char balance;
} avl_node;

typedef struct AVL_TREE {
    avl_node *root;
    unsigned long long length;
} avl_tree;

/* ********************* function declaration(S) SECTION ********************** */

