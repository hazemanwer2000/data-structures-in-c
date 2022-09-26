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

/**
 *  @brief      : 
**/
#define LEFT 	((void *) 0x1)
#define RIGHT 	((void *) 0x0)

/**
 *  @brief      : 
**/
#define BAL 	0
#define LHIGH 	1
#define RHIGH 	-1

/* ********************* static function declaration(s) SECTION ********************** */

static avl_node * avl_create_node(DATA_TYPE data);
static avl_node * avl_get_node(avl_tree *tree, LENGTH_DT i);

static avl_node * left_balance(avl_node *node);
static avl_node * right_balance(avl_node *node);
static avl_node * rotate_left(avl_node *node);
static avl_node * rotate_right(avl_node *node);

static void putchar_n(char c, unsigned int n);

/* ********************* function definition(s) SECTION ********************** */

/**
 *  @brief      : Create (dynamically, on heap) and intialize an AVL tree, and return a pointer to it.
 *  @param      : None.
 *  @return     : Pointer to tree.
**/
avl_tree * avl_create() {
    avl_tree *new_tree = (avl_tree *) malloc(sizeof(avl_tree));
    new_tree->length = 0, new_tree->root = NULL;
    return new_tree;
}

/**
 *  @brief      : (internal use only) Create a node (dynamically, on heap) and initialize it with data, etc, 
 * 					then return pointer to it.
 *  @param      : [ Data to store. ]
 *  @return     : Pointer to node.
**/
static avl_node * avl_create_node(DATA_TYPE data) {
    avl_node *new_node = (avl_node *) malloc(sizeof(avl_node));
    new_node->rchild = new_node->lchild = NULL;
    new_node->balance = 0, new_node->data = data;
    return new_node;
}

/**
 *  @brief      : 
 *  @param      : 
 *  @return     : 
**/
DATA_TYPE avl_get(avl_tree *tree, LENGTH_DT i) {
	avl_node *node = avl_get_node(tree, i);
	if (node != NULL) {
		return node->data;
	}
	return DEFAULT_VALUE;
}

/**
 *  @brief      : 
 *  @param      : 
 *  @return     : 
**/
static avl_node * avl_get_node(avl_tree *tree, LENGTH_DT i) {
	ll_list *stack = ll_create();
	LENGTH_DT j = 0;
	avl_node *node = NULL;
	avl_node *curr_node = tree->root;

	while (curr_node != NULL || stack->length != 0) {
		while (curr_node != NULL) {
			ll_push(stack, curr_node);
			curr_node = curr_node->lchild;
		}
		curr_node = ll_pop(stack);
		if (j++ == i) {
			node = curr_node;
			break;
		}
		curr_node = curr_node->rchild;
	}
	ll_destroy(stack);
	return node;
}

/**
 *  @brief      : 
 *  @param      : 
 *  @return     : 
**/
void avl_insert_unbalanced(avl_tree *tree, DATA_TYPE data, unsigned char (*f_compare)(DATA_TYPE new_data, DATA_TYPE old_data)) {
    avl_node *new_node = avl_create_node(data);
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

/**
 *  @brief      : 
 *  @param      : 
 *  @return     : 
**/
void avl_insert(avl_tree *tree, DATA_TYPE data, unsigned char (*f_compare)(DATA_TYPE new_data, DATA_TYPE old_data)) {
	unsigned int signal = 1;
	avl_node *curr_node = tree->root;
	avl_node *prev_node = NULL;
	ll_list *stack = ll_create();

	while (curr_node != NULL) {
		ll_push(stack, (void *) curr_node);
		if (f_compare(data, curr_node->data)) {
			ll_push(stack, LEFT);
			curr_node = curr_node->lchild;
		} else {
			ll_push(stack, RIGHT);
			curr_node = curr_node->rchild;
		}
	}

	curr_node = avl_create_node(data);

	while (stack->length != 0) {
		void * left_or_right = ll_pop(stack);
		prev_node = ll_pop(stack);
		if (left_or_right == LEFT) {
			prev_node->lchild = curr_node;
			if (signal) {
				if (prev_node->balance == RHIGH) {
					prev_node->balance = BAL;
					signal = 0;
				} else if (prev_node->balance == BAL) {
					prev_node->balance = LHIGH;
				} else {
					prev_node = left_balance(prev_node);          /* 2x LHIGH */
					signal = 0;
				}
			}
		} else {
			prev_node->rchild = curr_node;
			if (signal) {
				if (prev_node->balance == LHIGH) {
					prev_node->balance = BAL;
					signal = 0;
				} else if (prev_node->balance == BAL) {
					prev_node->balance = RHIGH;
				} else {
					prev_node = right_balance(prev_node);          /* 2x RHIGH */
					signal = 0;
				}
			}
		}
		curr_node = prev_node;
	}

	tree->root = curr_node;
	tree->length++;
	ll_destroy(stack);
}

/**
 *  @brief      : 
 *  @param      : 
 *  @return     : 
**/
void avl_delete_unbalanced(avl_tree *tree, LENGTH_DT i, unsigned char (*f_compare)(DATA_TYPE new_data, DATA_TYPE old_data)) {
	avl_node *node_to_del = avl_get_node(tree, i);
	if (node_to_del != NULL) {
		avl_node **parent_ptr = &tree->root;
		while (1) {
			if (*parent_ptr == node_to_del) {
				if ((*parent_ptr)->lchild == NULL && (*parent_ptr)->rchild ==  NULL) {    	/* Case: No children. */
					free(*parent_ptr);
					*parent_ptr = NULL;
					break;
				} else if ((*parent_ptr)->lchild == NULL) {									/* Case: Right child. */
					avl_node *tmp = *parent_ptr;
					*parent_ptr = tmp->rchild;
					free(tmp);
					break;
				} else if ((*parent_ptr)->lchild == NULL) {									/* Case: Left child. */
					avl_node *tmp = *parent_ptr;
					*parent_ptr = tmp->lchild;
					free(tmp);
					break;
				} else {																	/* Case: Two children. */
					avl_node **tmp_ptr = &(*parent_ptr)->rchild;				/* get next in-order */
					while ((*tmp_ptr)->lchild != NULL) {
						tmp_ptr = &(*tmp_ptr)->lchild;
					}
					(*parent_ptr)->data = (*tmp_ptr)->data;
					parent_ptr = tmp_ptr;
					node_to_del = *tmp_ptr;
				}
			} else {
				if (f_compare(node_to_del->data, (*parent_ptr)->data)) {
					parent_ptr = &(*parent_ptr)->lchild;
				} else {
					parent_ptr = &(*parent_ptr)->rchild;
				}
			}
		}
	}
}

/**
 *  @brief      : 
 *  @param      : 
 *  @return     : 
**/
void avl_delete(avl_tree *tree, LENGTH_DT i, unsigned char (*f_compare)(DATA_TYPE new_data, DATA_TYPE old_data)) {
	avl_node *node_to_del = avl_get_node(tree, i);
	if (node_to_del != NULL) {
		avl_node **parent_ptr = &tree->root;
		while (1) {
			if (*parent_ptr == node_to_del) {
				if ((*parent_ptr)->lchild == NULL && (*parent_ptr)->rchild ==  NULL) {    	/* Case: No children. */
					free(*parent_ptr);
					*parent_ptr = NULL;
					break;
				} else if ((*parent_ptr)->lchild == NULL) {									/* Case: Right child. */
					avl_node *tmp = *parent_ptr;
					*parent_ptr = tmp->rchild;
					free(tmp);
					break;
				} else if ((*parent_ptr)->lchild == NULL) {									/* Case: Left child. */
					avl_node *tmp = *parent_ptr;
					*parent_ptr = tmp->lchild;
					free(tmp);
					break;
				} else {																	/* Case: Two children. */
					avl_node **tmp_ptr = &(*parent_ptr)->rchild;				/* get next in-order */
					while ((*tmp_ptr)->lchild != NULL) {
						tmp_ptr = &(*tmp_ptr)->lchild;
					}
					(*parent_ptr)->data = (*tmp_ptr)->data;
					parent_ptr = tmp_ptr;
					node_to_del = *tmp_ptr;
				}
			} else {
				if (f_compare(node_to_del->data, (*parent_ptr)->data)) {
					parent_ptr = &(*parent_ptr)->lchild;
				} else {
					parent_ptr = &(*parent_ptr)->rchild;
				}
			}
		}
	}
}

/**
 *  @brief      : 
 *  @param      : 
 *  @return     : 
**/
static avl_node * left_balance(avl_node *node) {
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
	return node;
}

/**
 *  @brief      : 
 *  @param      : 
 *  @return     : 
**/
static avl_node * right_balance(avl_node *node) {
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
	return node;
}

/**
 *  @brief      : 
 *  @param      : 
 *  @return     : 
**/
static avl_node * rotate_left(avl_node *node) {
	avl_node *tmp = node->rchild;
	node->rchild = tmp->lchild;
	tmp->lchild = node;
	return tmp;
}

/**
 *  @brief      : 
 *  @param      : 
 *  @return     : 
**/
static avl_node * rotate_right(avl_node *node) {
	avl_node *tmp = node->lchild;
	node->lchild = tmp->rchild;
	tmp->rchild = node;
	return tmp;
}

/**
 *  @brief      : 
 *  @param      : 
 *  @return     : 
**/
LENGTH_DT avl_height(avl_tree *tree) {
	ll_list *queue = ll_create();
	LENGTH_DT height = 0;
	avl_node *node = NULL;

	if (tree->root != NULL) {
		ll_enqueue(queue, tree->root);
		while (queue->length != 0) {
			height++;
			LENGTH_DT fixed_length = queue->length;
			while (fixed_length-- != 0) {
				node = ll_dequeue(queue);
				if (node->lchild != NULL) { ll_enqueue(queue, node->lchild); }
				if (node->rchild != NULL) { ll_enqueue(queue, node->rchild); }
			}
		}
	}
	ll_destroy(queue);
	return height;
}

/**
 *  @brief      : 
 *  @param      : 
 *  @return     : 
**/
ll_list * avl_tree_to_list(avl_tree *tree) {
	ll_list *list = ll_create();
	ll_list *stack = ll_create();

	avl_node *curr_node = tree->root;
	while (curr_node != NULL || stack->length != 0) {
		while (curr_node != NULL) {
			ll_push(stack, curr_node);
			curr_node = curr_node->lchild;
		}
		curr_node = ll_pop(stack);
		ll_append(list, curr_node->data);
		curr_node = curr_node->rchild;
	}
	ll_destroy(stack);
	return list;
}

/**
 *  @brief      : 
 *  @param      : 
 *  @return     : 
**/
void avl_print(avl_tree *tree, void (*f_print)(DATA_TYPE data), unsigned char unit_size) {
	LENGTH_DT height = avl_height(tree);
	ll_list *queue = ll_create();
	avl_node *node = NULL;
	unsigned int factor = 0;
	LENGTH_DT fixed_length;

	for (int i = 1; i < height; i++) { factor = factor * 2 + 1; }
	ll_enqueue(queue, tree->root);
	while (height-- != 0) {
		fixed_length = queue->length;
		while (fixed_length-- != 0) {
			node = ll_dequeue(queue);
			if (node != NULL) {
				putchar_n(' ', factor*unit_size);
				f_print(node->data);
				putchar_n(' ', (factor+1)*unit_size);
				ll_enqueue(queue, node->lchild);
				ll_enqueue(queue, node->rchild);
			} else {
				putchar_n(' ', (factor + 1 << 1)*unit_size);
				ll_enqueue(queue, NULL);
				ll_enqueue(queue, NULL);
			}
		}
		factor = factor - 1 >> 1;
		putchar('\n');
	}
	ll_destroy(queue);
}

/**
 *  @brief      : 
 *  @param      : 
 *  @return     : 
**/
static void putchar_n(char c, unsigned int n) {
	while (n-- > 0) { putchar(c); }
}

/* ********************* 'main' function defintion SECTION (UNIT-TEST) ********************** */

#ifdef _MAIN_AVL_TREE_ 					/* compile-time switch */

#define LEN(ARR) (*(&ARR+1)-ARR)

int * test_int(int x);

unsigned char f_compare(DATA_TYPE new_data, DATA_TYPE old_data);
void f_clean(avl_tree *tree);
void f_clean_ll(ll_list *list);
void f_print(void *data);
void f_print_avl(void *data);

int main() {
	avl_tree * tree = avl_create();
	int to_insert[] = {1, 2, 3, 4, 5, 6, 7, 8}; 			// {8, 4, 12, 2, 6, 10, 14, 1, 3, 5, 7, 9, 11, 13, 15};
	for (int i = 0; i < LEN(to_insert); i++) {
		printf("Inserting...\n");
		avl_insert(tree, test_int(to_insert[i]), f_compare);
	}
	ll_print(avl_tree_to_list(tree), f_print, f_clean_ll);
	printf("\nHeight: %lu\n", avl_height(tree));
	avl_print(tree, f_print_avl, 4);

	printf("List using get (length: %ld)\n", tree->length);
	for (int i = 0; i < tree->length; i++) {
		printf("%d ", *((int *) avl_get(tree, i)));
	}
	putchar('\n');

	avl_delete_unbalanced(tree, 5, f_compare);
	avl_print(tree, f_print_avl, 4);

	return 0;
}

int * test_int(int x) {
	int * x_ptr = malloc(sizeof(int));
	*x_ptr = x;
	return x_ptr;
}

unsigned char f_compare(void *new_data, void *old_data) {
	return *((int *) new_data) < *((int *) old_data) ? 1 : 0;
}

void f_clean_ll(ll_list *list) {
	printf("\b\b \n");
}

void f_clean(avl_tree *tree) {
	printf("\b\b \n");
}

void f_print(void *data) {
	printf("%d, ", *((int *) data));
}

void f_print_avl(void *data) {
	printf("[%2d]", *((int *) data));
}

#endif