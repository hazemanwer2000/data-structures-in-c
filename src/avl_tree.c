/**
 ****************************************************************
 * @file			: avl_tree.c
 * @author 			: Eng. Hazem Mostafa Abdelaziz Anwer
 * @brief 			: Implementation of an AVL Binary Search Tree (BST).
 * **************************************************************
 **/

/* ********************* #include SECTION ********************** */

#include "avl_tree.h"

/* ********************* #define SECTION ********************** */

#define MAX(A, B) (A >= B ? A : B)

#define PTREE_PRE 			"["
#define PTREE_POST 			"]"
#define PTREE_DIGITS 		"2"
#define PTREE_TOTAL 		4
#define PTREE_BALANCE		1

/* ********************* static function declaration(s) SECTION ********************** */

static avl_node * avl_create_node(avl_tree *tree, DATA_TYPE data);
static LENGTH_DT avl_height_helper(avl_node *node, LENGTH_DT level);
static void avl_print_list(avl_tree *tree, void (*func_print)(DATA_TYPE data), void (*func_clean)());
static void avl_print_list_helper(avl_node *node, void (*func_print)(DATA_TYPE data));
static void avl_print_tree(avl_tree *tree);
static void avl_print_tree_level(avl_node *node, LENGTH_DT curr_height, LENGTH_DT goal_height, unsigned int gap_size);
static unsigned int avl_print_tree_gap_size(LENGTH_DT height);
static void putchar_n(char c, unsigned int n);

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

LENGTH_DT avl_height(avl_tree *tree) {
	return avl_height_helper(tree->root, 0);
}

static LENGTH_DT avl_height_helper(avl_node *node, LENGTH_DT level) {
	if (node != NULL) {
		level++;
		return MAX(MAX(level, avl_height_helper(node->lchild, level)), avl_height_helper(node->rchild, level));
	}
	return level;
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

static void avl_print_tree(avl_tree *tree) {
	LENGTH_DT max_height = avl_height(tree);
	for (LENGTH_DT goal_height = max_height; goal_height > 0; goal_height--) {
		avl_print_tree_level(tree->root, max_height, goal_height, PTREE_TOTAL*avl_print_tree_gap_size(goal_height));
		putchar('\n');
	}
}

static void avl_print_tree_level(avl_node *node, LENGTH_DT curr_height, LENGTH_DT goal_height, unsigned int gap_size) {
	if (curr_height > 0) {
		if (node != NULL) {
			if (curr_height == goal_height) {
				putchar_n(' ', gap_size);
				printf(PTREE_PRE "%" PTREE_DIGITS "d" PTREE_POST,
						#if PTREE_BALANCE
							node->balance
						#else
							node->data
						#endif
					);
				putchar_n(' ', gap_size+PTREE_TOTAL);
			}
			avl_print_tree_level(node->lchild, curr_height-1, goal_height, gap_size);
			avl_print_tree_level(node->rchild, curr_height-1, goal_height, gap_size);
		} else {
			if (curr_height == goal_height) {
				putchar_n(' ', 2*(gap_size+PTREE_TOTAL));
			}
			avl_print_tree_level(NULL, curr_height-1, goal_height, gap_size);
			avl_print_tree_level(NULL, curr_height-1, goal_height, gap_size);
		}
	}
}

static unsigned int avl_print_tree_gap_size(LENGTH_DT height) {
	unsigned int gap = 0;
	while (--height > 0) {
		gap = gap * 2 + 1;
	}
	return gap;
}

static void putchar_n(char c, unsigned int n) {
	while (n-- > 0) {
		putchar(c);
	}
}

/* ********************* 'main' function defintion SECTION (UNIT-TEST) ********************** */

#ifdef _MAIN_AVL_TREE_

#define LEN(ARR) (*(&ARR+1)-ARR)

unsigned char f_compare(DATA_TYPE new_data, DATA_TYPE old_data);
void f_clean(avl_tree *tree);
void f_print(int data);

int main() {
	avl_tree *tree = avl_create();

	int to_insert[] = {4, 2, 6, 1, 3, 5, 7};
    for (int i = 0; i < LEN(to_insert); i++) {
        printf("Inserting: %d\n", to_insert[i]);
        avl_insert(tree, to_insert[i], f_compare);
        avl_print_list(tree, f_print, f_clean);
    }
    putchar('\n');
	avl_print_tree(tree);
	putchar('\n');

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