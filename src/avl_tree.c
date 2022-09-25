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

/* ********************* static function declaration(s) SECTION ********************** */

static avl_node * avl_create_node(DATA_TYPE data);
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
	int to_insert[] = {8, 4, 12, 2, 6, 10, 14, 1, 3, 5, 7, 9, 11, 13, 15};
	for (int i = 0; i < LEN(to_insert); i++) {
		avl_insert_unbalanced(tree, test_int(to_insert[i]), f_compare);
	}
	ll_print(avl_tree_to_list(tree), f_print, f_clean_ll);
	printf("\nHeight: %lu\n", avl_height(tree));
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