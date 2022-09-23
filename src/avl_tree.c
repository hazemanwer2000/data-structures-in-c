/**
 ****************************************************************
 * @file			: avl_tree.c
 * @author 			: Eng. Hazem Mostafa Abdelaziz Anwer
 * @brief 			: Implementation of an AVL Binary Search Tree (BST).
 * **************************************************************
 **/

/* ********************* #include SECTION ********************** */

#include "avl_tree.h"

/* ********************* static function declaration(s) SECTION ********************** */



/* ********************* function definition(s) SECTION ********************** */

avl_tree * avl_create() {
    avl_tree *new_tree = (avl_tree *) malloc(sizeof(avl_tree));
    new_tree->length = 0;
    new_tree->root = NULL;
    return new_tree;
}

void avl_insert(avl_tree *tree, DATA_TYPE data) {
    avl_node *new_node = (avl_node *) malloc(sizeof(avl_node));
    new_node->rchild = NULL;
    new_node->lchild = NULL;
    new_node->balance = 0;
    new_node->data = data;

    avl_node *parent = &tree->root;
    while (*parent != NULL) {
    	if (new_node->data < *parent->data) {
    		parent = parent->lchild;
    	} else {
    		parent = parent->rchild;
    	}
    }
    *parent = new_node;
    tree->length++;
}

void avl_print_list(avl_tree *tree, void (*func_print)(DATA_TYPE data), void (*func_clean)()) {
	avl_print_list_helper(tree->root, func_print);
	func_clean();
}

static avl_print_list_helper(avl_node *node, void (*func_print)(DATA_TYPE data)) {
	if (node != NULL) {
		avl_print_list_helper(node->lchild);
		func_print(node->data);
		avl_print_list_helper(node->rchild);
	}
}