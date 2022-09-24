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

static avl_node * avl_create_node(avl_tree *tree, DATA_TYPE data);
static void avl_print_list(avl_tree *tree, void (*func_print)(DATA_TYPE data), void (*func_clean)());
static void avl_print_list_helper(avl_node *node, void (*func_print)(DATA_TYPE data));
static void avl_print_tree(avl_tree *tree, void (*func_print)(DATA_TYPE data));

/* ********************* function definition(s) SECTION ********************** */

avl_tree * avl_create() {
    avl_tree *new_tree = (avl_tree *) malloc(sizeof(avl_tree));
    new_tree->length = 0, new_tree->root = NULL;
    return new_tree;
}

static avl_node * avl_create_node(avl_tree *tree, DATA_TYPE data) {
    avl_node *new_node = (avl_node *) malloc(sizeof(avl_node));
    new_node->rchild = new_node->lchild = NULL;
    new_node->balance = 0, new_node->data = data;
    return new_node;
}

void avl_insert(avl_tree *tree, DATA_TYPE data, unsigned char (*f_compare)(DATA_TYPE new_data, DATA_TYPE old_data)) {
    avl_node *new_node = avl_create_node(tree, data);
    avl_node **parent = &tree->root;
    while (*parent != NULL) {
    	if (f_compare(new_node->data, (*parent)->data)) {
    		parent = &(*parent)->lchild;
    	} else {
    		parent = &(*parent)->rchild;
    	}
    }
    *parent = new_node;
    tree->length++;
}

static void avl_print_list(avl_tree *tree, void (*f_print)(DATA_TYPE data), void (*f_clean)(avl_tree *tree)) {
	avl_print_list_helper(tree->root, f_print);
	f_clean(tree);
}

static void avl_print_list_helper(avl_node *node, void (*f_print)(DATA_TYPE data)) {
	if (node != NULL) {
		avl_print_list_helper(node->lchild, f_print);
		f_print(node->data);
		avl_print_list_helper(node->rchild, f_print);
	}
}

static void avl_print_tree(avl_tree *tree, void (*f_print)(DATA_TYPE data)) {

}

/* ********************* 'main' function defintion SECTION (UNIT-TEST) ********************** */

#ifdef _MAIN_AVL_TREE_

unsigned char f_compare(DATA_TYPE new_data, DATA_TYPE old_data);
void f_clean(avl_tree *tree);
void f_print(int data);

int main() {
	avl_tree *tree = avl_create();
	avl_insert(tree, 5, f_compare);
	avl_insert(tree, 1, f_compare);
	avl_insert(tree, 8, f_compare);
	avl_print_list(tree, f_print, f_clean);
	return 0;
}

unsigned char f_compare(DATA_TYPE new_data, DATA_TYPE old_data) {
	if (new_data < old_data) {
		return 1;
	}
	return 0;
}

void f_clean(avl_tree *tree) {
	printf("\b\b \n");
}

void f_print(int data) {
	printf("%d, ", data);
}

#endif