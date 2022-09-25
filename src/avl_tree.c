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
#define PTREE_BALANCE		0

#define BAL 	0
#define LHIGH 	1
#define RHIGH 	-1

/* ********************* static function declaration(s) SECTION ********************** */

static avl_node * avl_create_node(avl_tree *tree, DATA_TYPE data);
static void avl_insert_BT(avl_tree *tree, DATA_TYPE data, unsigned char (*f_compare)(DATA_TYPE new_data, DATA_TYPE old_data));
static avl_node * avl_insert_rec(avl_node *root_node, avl_node *new_node, unsigned int *signal,
								unsigned char (*f_compare)(DATA_TYPE new_data, DATA_TYPE old_data));
static avl_node * left_balance(avl_node *node, unsigned int *signal);
static avl_node * right_balance(avl_node *node, unsigned int *signal);
static avl_node * rotate_left(avl_node *node);
static avl_node * rotate_right(avl_node *node);
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

static void avl_insert_BT(avl_tree *tree, DATA_TYPE data, unsigned char (*f_compare)(DATA_TYPE new_data, DATA_TYPE old_data)) {
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

void avl_insert(avl_tree *tree, DATA_TYPE data, unsigned char (*f_compare)(DATA_TYPE new_data, DATA_TYPE old_data)) {
	unsigned int signal;
	tree->root = avl_insert_rec(tree->root, avl_create_node(tree, data), &signal, f_compare);
	tree->length++;
}

static avl_node * avl_insert_rec(avl_node *root_node, avl_node *new_node, unsigned int *signal,
								unsigned char (*f_compare)(DATA_TYPE new_data, DATA_TYPE old_data)) {
	if (root_node == NULL) {
		root_node = new_node;
		*signal = 1;
	} else if (f_compare(new_node->data, root_node->data)) {
		root_node->lchild = avl_insert_rec(root_node->lchild, new_node, signal, f_compare);
		if (*signal) {
			if (root_node->balance == RHIGH) {
				root_node->balance = BAL;
				*signal = 0;
			} else if (root_node->balance == BAL) {
				root_node->balance = LHIGH;
			} else {
				root_node = left_balance(root_node, signal);          /* 2x LHIGH */
			}
		}
	} else {
		root_node->rchild = avl_insert_rec(root_node->rchild, new_node, signal, f_compare);
		if (*signal) {
			if (root_node->balance == LHIGH) {
				root_node->balance = BAL;
				*signal = 0;
			} else if (root_node->balance == BAL) {
				root_node->balance = RHIGH;
			} else {
				root_node = right_balance(root_node, signal);          /* 2x RHIGH */
			}
		}
	}
	return root_node;
}

static avl_node * left_balance(avl_node *node, unsigned int *signal) {
	avl_node *lsub, *lrsub;
	lsub = node->lchild;
	switch (lsub->balance) {
		case LHIGH:													/* single rotation */
			node->balance = BAL;
			lsub->balance = BAL;
			node = rotate_right(node);
			break;
		case RHIGH:													/* double rotation */
			lrsub = lsub->rchild;
			switch(lrsub->balance) {
				case LHIGH:
					node->balance = RHIGH;
					lsub->balance = BAL;
					break;
				case BAL:
					node->balance = BAL;
					lsub->balance = BAL;
					break;
				case RHIGH:
					node->balance = BAL;
					lsub->balance =	LHIGH;
					break;
			}
			lrsub->balance = BAL;
			node->lchild = rotate_left(lsub);
			node = rotate_right(node);
			break;
	}
	*signal = 0;
	return node;
}

static avl_node * right_balance(avl_node *node, unsigned int *signal) {
	avl_node *rsub, *rlsub;
	rsub = node->rchild;
	switch (rsub->balance) {
		case RHIGH:													/* single rotation */
			node->balance = BAL;
			rsub->balance = BAL;
			node = rotate_left(node);
			break;
		case LHIGH:													/* double rotation */
			rlsub = rsub->lchild;
			switch(rlsub->balance) {
				case RHIGH:
					node->balance = LHIGH;
					rsub->balance = BAL;
					break;
				case BAL:
					node->balance = BAL;
					rsub->balance = BAL;
					break;
				case LHIGH:
					node->balance = BAL;
					rsub->balance =	RHIGH;
					break;
			}
			rlsub->balance = BAL;
			node->rchild = rotate_right(rsub);
			node = rotate_left(node);
			break;
	}
	*signal = 0;
	return node;
}

static avl_node * rotate_left(avl_node *node) {
	avl_node *tmp = node->rchild;
	node->rchild = tmp->lchild;
	tmp->lchild = node;
	return tmp;
}

static avl_node * rotate_right(avl_node *node) {
	avl_node *tmp = node->lchild;
	node->lchild = tmp->rchild;
	tmp->rchild = node;
	return tmp;
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

void test_insert_BT();
void test_insert();
void test_insert_helper(avl_tree *tree, int to_insert[], int len);
unsigned char f_compare(DATA_TYPE new_data, DATA_TYPE old_data);
void f_clean(avl_tree *tree);
void f_print(int data);

int main() {
	/* test_insert_BT(); */
	test_insert();
	return 0;
}

void test_insert() {
	avl_tree *tree = avl_create();
	printf("\n\n*********** TESTING (INSERT-LEFT-ROTATION) ***********\n\n");
	int to_insert1[] = {1, 2, 3, 4, 5, 6, 7};
	test_insert_helper(tree, to_insert1, LEN(to_insert1));

	tree = avl_create();
	printf("\n\n*********** TESTING (INSERT-RIGHT-ROTATION) ***********\n\n");
	int to_insert2[] = {7, 6, 5, 4, 3, 2, 1};
	test_insert_helper(tree, to_insert2, LEN(to_insert2));

	tree = avl_create();
	printf("\n\n*********** TESTING (INSERT-RIGHT-LEFT-ROTATION-1) ***********\n\n");
	int to_insert3[] = {2, 1, 5, 3, 6, 4};
	test_insert_helper(tree, to_insert3, LEN(to_insert3));

	tree = avl_create();
	printf("\n\n*********** TESTING (INSERT-RIGHT-LEFT-ROTATION-2) ***********\n\n");
	int to_insert4[] = {2, 1, 5, 4, 6, 3};
	test_insert_helper(tree, to_insert4, LEN(to_insert4));

	tree = avl_create();
	printf("\n\n*********** TESTING (INSERT-RIGHT-LEFT-ROTATION-3) ***********\n\n");
	int to_insert5[] = {1, 3, 2};
	test_insert_helper(tree, to_insert5, LEN(to_insert5));

	tree = avl_create();
	printf("\n\n*********** TESTING (INSERT-LEFT-RIGHT-ROTATION-1) ***********\n\n");
	int to_insert6[] = {5, 2, 6, 1, 3, 4};
	test_insert_helper(tree, to_insert6, LEN(to_insert6));

	tree = avl_create();
	printf("\n\n*********** TESTING (INSERT-LEFT-RIGHT-ROTATION-2) ***********\n\n");
	int to_insert7[] = {5, 2, 6, 1, 4, 3};
	test_insert_helper(tree, to_insert7, LEN(to_insert7));

	tree = avl_create();
	printf("\n\n*********** TESTING (INSERT-LEFT-RIGHT-ROTATION-3) ***********\n\n");
	int to_insert8[] = {3, 1, 2};
	test_insert_helper(tree, to_insert8, LEN(to_insert8));
}

void test_insert_helper(avl_tree *tree, int to_insert[], int len) {
    for (int i = 0; i < len; i++) {
        printf("Inserting: %d\n", to_insert[i]);
        avl_insert(tree, to_insert[i], f_compare);
		avl_print_tree(tree);
		putchar('\n');
    }
}

void test_insert_BT() {
	avl_tree *tree = avl_create();

	int to_insert[] = {2, 5, 8, 1, 3, 0};           /* {4, 2, 6, 1, 3, 5, 7} level-first */
    for (int i = 0; i < LEN(to_insert); i++) {
        printf("Inserting: %d\n", to_insert[i]);
        avl_insert_BT(tree, to_insert[i], f_compare);
        avl_print_list(tree, f_print, f_clean);
    }
    putchar('\n');
	avl_print_tree(tree);
	putchar('\n');
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