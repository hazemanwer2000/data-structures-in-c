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

static void avl_print_list_helper(avl_node *node, void (*func_print)(DATA_TYPE data));

/* ********************* 'main' function defintion SECTION (UNIT-TEST) ********************** */

#ifdef _MAIN_AVL_TREE_

void func_clean();
void func_print(int data);

int main() {
	avl_tree *tree = avl_create();
	avl_insert(tree, 5);
	avl_insert(tree, 1);
	avl_insert(tree, 8);
	avl_print_list(tree, func_print, func_clean);
	return 0;
}

void func_clean() {
	printf("Cleaning...");
}

void func_print(int data) {
	printf("%d, ", data);
}

#endif

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

    avl_node **parent = &tree->root;
    while (*parent != NULL) {
    	if (new_node->data < (*parent)->data) {
    		parent = &(*parent)->lchild;
    	} else {
    		parent = &(*parent)->rchild;
    	}
    }
    *parent = new_node;
    tree->length++;
}

void avl_print_list(avl_tree *tree, void (*func_print)(DATA_TYPE data), void (*func_clean)()) {
	avl_print_list_helper(tree->root, func_print);
	func_clean();
}

static void avl_print_list_helper(avl_node *node, void (*func_print)(DATA_TYPE data)) {
	if (node != NULL) {
		avl_print_list_helper(node->lchild, func_print);
		func_print(node->data);
		avl_print_list_helper(node->rchild, func_print);
	}
}

void avl_print_tree(avl_tree *tree, void (*func_print)(DATA_TYPE data)) {

}

static void avl_print_list_helper(avl_node *node, avl_node void (*func_print)(DATA_TYPE data)) {
	if (node != NULL) {
		avl_print_list_helper(node->lchild, func_print);
		func_print(node->data);
		avl_print_list_helper(node->rchild, func_print);
	}
}