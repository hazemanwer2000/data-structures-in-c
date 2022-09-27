/**
 ****************************************************************
 * @file            : avl_tree.c
 * @author          : Eng. Hazem Mostafa Abdelaziz Anwer
 * @brief           : Implementation of an AVL Binary Search Tree (BST).
 * **************************************************************
 **/

/* ********************* #include SECTION ********************** */

#include "avl_tree.h"

/* ********************* #define SECTION ********************** */

/**
 *  @brief      : Signals, encoded as 'void *', to be stored in lists, used in iterative implementations
 *                  of AVL insertions and deletions.
**/
#define LEFT    ((void *) 0x1)
#define RIGHT   ((void *) 0x0)

/**
 *  @brief      : Macro definitions of possible balance values (e.g: 'LHIGH' means Left-side higher by one).
**/
#define BAL         0
#define LHIGH       1
#define RHIGH       -1

/* ********************* static function declaration(s) SECTION ********************** */

static avl_node * avl_create_node(DATA_TYPE data);
static avl_node * avl_get_node(avl_tree *tree, LENGTH_DT i);
static void avl_deallocate_all(avl_tree *tree);

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
 *                  then return pointer to it.
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
 *  @brief      : Returns data stored at a specific index. If the index is out of bounds, returns DEFAULT_VALUE
 *                  set in the header file. Utilizes 'avl_get_node' function.
 *  @param      : [ Tree. ]
 *                [ Index. ]
 *  @return     : Data stored.
**/
DATA_TYPE avl_get(avl_tree *tree, LENGTH_DT i) {
    avl_node *node = avl_get_node(tree, i);
    if (node != NULL) {
        return node->data;
    }
    return DEFAULT_VALUE;
}

/**
 *  @brief      : (for internal use) Returns a node at a specific index. If the index is out of bounds, returns NULL.
 *                  In-order traversal is performed using a stack (iterative solution, not recursive. The left-child 
 *                  of the current element is continously pushed onto the stack, and set as the current element, until 
 *                  the current element is NULL. In that case, an item is popped, consumed (i++), and then its right-child
 *                  is set as the current element. The process then repeats while the current element is not NULL, or
 *                  the stack is not empty.
 *  @param      : [ Tree, ] 
 *                [ Index. ]
 *  @return     : Pointer to node.
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
 *  @brief      : Inserts new data into a tree, like a sorted binary tree, and not an AVL BST. Continously traverses
 *                  until a node is NULL. Then sets it to the new node, containing the data. A function must be passed
 *                  as a parameter. During the traversal, the function is called on the new data, and the data of the
 *                  node currently being traversed. The function should return '1' if the new data resides on the left
 *                  of the traverse node, and '0' if it resides on the right of.
 *  @param      : [ Tree. ]
 *                [ Data to insert. ]
 *                [ Function that receives the new data and the data of the current traverse node, and returns 0 (right) or 1 (left). ]
 *  @return     : None.
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
 *  @brief      : Inserts a new data into the tree, balancing the tree thereafter (AVL BST style). A stack is used to
 *                  store each node along the traversal path, following the way of a sorted binary tree. Items are
 *                  then continously popped off (along with the encoded direction of traversal), and adjusted
 *                  based on the AVL insertion algorithm.
 *                  (Note: AVL insertion algorithm is complex, and demands a reference to understand.)
 *                  (Note: For information on 'f_compare', read '@brief' of 'avl_insert_unbalanced'.)
 *  @param      : [ Tree. ]
 *                [ Data to insert. ]
 *                [ Function that receives the new data and the data of the current traverse node, and returns 0 (right) or 1 (left). ]
 *  @return     : None.
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
 *  @brief      : Deletes an item at an index. Uses the sorted binary tree deletion algorithm, and not that of
 *                  an AVL BST. Four cases are adhered to: No children, right child only, left child only, and
 *                  two children. In the last case, the next in-order node is located and its data is copied to the
 *                  previously to-be-deleted node, then that (in-order) node is itself deleted instead.
 *                  (Note: For information on 'f_compare', read '@brief' of 'avl_insert_unbalanced'.)
 *  @param      : [ Tree. ]
 *                [ Index to delete at. ]
 *                [ Function that receives the new data and the data of the current traverse node, and returns 0 (right) or 1 (left). ]
 *  @return     : None.
**/
void avl_delete_unbalanced(avl_tree *tree, LENGTH_DT i, unsigned char (*f_compare)(DATA_TYPE new_data, DATA_TYPE old_data)) {
    avl_node *node_to_del = avl_get_node(tree, i);
    if (node_to_del != NULL) {
        avl_node **parent_ptr = &tree->root;
        while (1) {
            if (*parent_ptr == node_to_del) {
                if ((*parent_ptr)->lchild == NULL && (*parent_ptr)->rchild == NULL) {       /* Case: No children. */
                    free(*parent_ptr);
                    *parent_ptr = NULL;
                    break;
                } else if ((*parent_ptr)->lchild == NULL) {                                 /* Case: Right child. */
                    avl_node *tmp = *parent_ptr;
                    *parent_ptr = tmp->rchild;
                    free(tmp);
                    break;
                } else if ((*parent_ptr)->lchild == NULL) {                                 /* Case: Left child. */
                    avl_node *tmp = *parent_ptr;
                    *parent_ptr = tmp->lchild;
                    free(tmp);
                    break;
                } else {                                                                    /* Case: Two children. */
                    avl_node **tmp_ptr = &(*parent_ptr)->rchild;                /* get next in-order */
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
        tree->length--;
    }
}

/**
 *  @brief      : Deletes an item at an index. Uses AVL BST deletion algorithm. It builds upon 'avl_delete unbalanced',
 *                  by tracing the nodes back along the traversal path, and re-balancing.
 *                  (Note: AVL deletion algorithm is complex, and demands a reference to understand.)
 *                  (Note: For information on 'f_compare', read '@brief' of 'avl_insert_unbalanced'.)
 *  @param      : [ Tree. ]
 *                [ Index to delete at. ]
 *                [ Function that receives the new data and the data of the current traverse node, and returns 0 (right) or 1 (left). ]
 *  @return     : None.
**/
void avl_delete(avl_tree *tree, LENGTH_DT i, unsigned char (*f_compare)(DATA_TYPE new_data, DATA_TYPE old_data)) {
    avl_node *node_to_del = avl_get_node(tree, i);
    if (node_to_del != NULL) {
        ll_list *stack = ll_create();
        avl_node **parent_ptr = &tree->root;
        while (1) {
            if (*parent_ptr == node_to_del) {
                if ((*parent_ptr)->lchild == NULL && (*parent_ptr)->rchild == NULL) {       /* Case: No children. */
                    free(*parent_ptr);
                    *parent_ptr = NULL;
                    break;
                } else if ((*parent_ptr)->lchild == NULL) {                                 /* Case: Right child. */
                    avl_node *tmp = *parent_ptr;
                    *parent_ptr = tmp->rchild;
                    free(tmp);
                    break;
                } else if ((*parent_ptr)->lchild == NULL) {                                 /* Case: Left child. */
                    avl_node *tmp = *parent_ptr;
                    *parent_ptr = tmp->lchild;
                    free(tmp);
                    break;
                } else {                                                                    /* Case: Two children. */
                    avl_node *tmp = (*parent_ptr)->rchild;              /* get next in-order */
                    while (tmp->lchild != NULL) {
                        tmp = tmp->lchild;
                    }
                    (*parent_ptr)->data = tmp->data;
                    parent_ptr = &tree->root;
                    node_to_del = tmp;
                    ll_delete_all(stack);
                }
            } else {
                ll_push(stack, (void *) parent_ptr);
                if (f_compare(node_to_del->data, (*parent_ptr)->data)) {
                    ll_push(stack, LEFT);
                    parent_ptr = &(*parent_ptr)->lchild;
                } else {
                    ll_push(stack, RIGHT);
                    parent_ptr = &(*parent_ptr)->rchild;
                }
            }
        }

        while (stack->length != 0) {
            void *left_or_right = ll_pop(stack);
            avl_node **parent_ptr = (avl_node **) ll_pop(stack);
            if (left_or_right == LEFT) {
                if ((*parent_ptr)->balance == BAL) {
                    (*parent_ptr)->balance = RHIGH;
                    break;
                } else if ((*parent_ptr)->balance == LHIGH) {
                    (*parent_ptr)->balance = BAL;
                } else {                                                /* 2x RHIGH */
                    *parent_ptr = right_balance(*parent_ptr);
                }
            } else {
                if ((*parent_ptr)->balance == BAL) {
                    (*parent_ptr)->balance = LHIGH;
                    break;
                } else if ((*parent_ptr)->balance == RHIGH) {
                    (*parent_ptr)->balance = BAL;
                } else {                                                /* 2x RHIGH */
                    *parent_ptr = right_balance(*parent_ptr);
                }
            }
        }
        tree->length--;
        ll_destroy(stack);
    }
}

/**
 *  @brief      : Left balance a 2x LHIGH node in an AVL tree.
 *  @param      : [ Node to rebalance. ]
 *  @return     : [ Node after rebalancing (may not be the same node). ]
**/
static avl_node * left_balance(avl_node *node) {
    avl_node *lsub, *lrsub;
    lsub = node->lchild;
    switch (lsub->balance) {
        case LHIGH:                                                 /* single rotation */
            node->balance = BAL;
            lsub->balance = BAL;
            node = rotate_right(node);
            break;
        case RHIGH:                                                 /* double rotation */
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
                    lsub->balance = LHIGH;
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
 *  @brief      : Left balance a 2x RHIGH node in an AVL tree.
 *  @param      : [ Node to rebalance. ]
 *  @return     : [ Node after rebalancing (may not be the same node). ]
**/
static avl_node * right_balance(avl_node *node) {
    avl_node *rsub, *rlsub;
    rsub = node->rchild;
    switch (rsub->balance) {
        case RHIGH:                                                 /* single rotation */
            node->balance = BAL;
            rsub->balance = BAL;
            node = rotate_left(node);
            break;
        case LHIGH:                                                 /* double rotation */
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
                    rsub->balance = RHIGH;
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
 *  @brief      : Left rotation (AVL BST terminology).
 *  @param      : [ Node to rotate. ]
 *  @return     : [ Node after rotation (may not be the same node). ]
**/
static avl_node * rotate_left(avl_node *node) {
    avl_node *tmp = node->rchild;
    node->rchild = tmp->lchild;
    tmp->lchild = node;
    return tmp;
}

/**
 *  @brief      : Right rotation (AVL BST terminology).
 *  @param      : [ Node to rotate. ]
 *  @return     : [ Node after rotation (may not be the same node). ]
**/
static avl_node * rotate_right(avl_node *node) {
    avl_node *tmp = node->lchild;
    node->lchild = tmp->rchild;
    tmp->rchild = node;
    return tmp;
}

/**
 *  @brief      : Get the height of a tree, implementing breadth-first (level-by-level) traversal, using a queue.
 *  @param      : [ Tree. ]
 *  @return     : Height of tree.
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
 *  @brief      : Returns a list from a tree, using in-order traversal through a stack. Tree is unmodified.
 *                  (Note: For more info on in-order traversal, check '@brief' at 'avl_get_node'.)
 *  @param      : [ Tree. ]
 *  @return     : [ List. ]
**/
ll_list * avl_make_list(avl_tree *tree) {
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
 *  @brief      : (for internal use) Deallocate all nodes in a tree, through breadth-first traversal (level-by-level),
 *                  using a queue. Does not reset the tree length of root pointer, or deallocate it.
 *  @param      : [ Tree. ]
 *  @return     : None.
**/
static void avl_deallocate_all(avl_tree *tree) {
    ll_list *queue = ll_create();
    avl_node *node = NULL;

    if (tree->root != NULL) {
        ll_enqueue(queue, tree->root);
        while (queue->length != 0) {
            LENGTH_DT fixed_length = queue->length;
            while (fixed_length-- != 0) {
                node = ll_dequeue(queue);
                if (node->lchild != NULL) { ll_enqueue(queue, node->lchild); }
                if (node->rchild != NULL) { ll_enqueue(queue, node->rchild); }
                free(node);
            }
        }
    }
    ll_destroy(queue);
}

/**
 *  @brief      : Deletes (free) all nodes in a tree, then resets the tree root pointer and length.
 *  @param      : [ Tree. ]
 *  @return     : None.
**/
void avl_delete_all(avl_tree *tree) {
    avl_deallocate_all(tree);
    tree->root = NULL, tree->length = 0;
}

/**
 *  @brief      : Deletes (free) all nodes in a tree, then deletes the tree itself.
 *  @param      : [ Tree. ]
 *  @return     : None.
**/
void avl_destroy(avl_tree *tree) {
    avl_deallocate_all(tree);
    free(tree);
}

/**
 *  @brief      : Prints a tree, level-by-level. It uses 'avl_height' initially determine the height of the tree,
 *                  and use it to format the output of the tree, according to a fixed pattern. It uses 
 *                  breadth-first traversal through a queue. 'f_print' receives the data at each node,
 *                  and is responsible to print it. It must print it to be of a consistent no. of chars. 
 *                  'unit_size' is the fixed no. of chars 'f_print' uses.
 *  @param      : [ Tree to print. ]
 *                [ Function that is passed the data at each node. ]
 *                [ Unit size (no. of chars) of each 'f_print' call. ]
 *  @return     : None.
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
 *  @brief      : Prints a 'char' a repeated number of times.
 *  @param      : [ 'char' to print. ]
 *                [ Number of repitions. ]
 *  @return     : None.
**/
static void putchar_n(char c, unsigned int n) {
    while (n-- > 0) { putchar(c); }
}

/* ********************* 'main' function defintion SECTION (UNIT-TEST) ********************** */

#ifdef _MAIN_AVL_TREE_                  /* compile-time switch */

#define LEN(ARR) (*(&ARR+1)-ARR)

unsigned char f_compare(void *new_data, void *old_data);
void f_print(void *data);

void t_insert_unbalanced();
void t_insert();

int main() {
    t_insert_unbalanced();
    /* t_insert(); */
    return 0;
}

void t_insert_unbalanced() {
    printf("*************** TEST (INSERT) ***************\n");
    avl_tree *tree = avl_create();
    int arr_data1[] = {3, 2, 5, 6, 8, 1, 9};
    for (int i = 0; i < LEN(arr_data1); i++) {
        printf("Inserting: %d\n", arr_data1[i]);
        avl_insert(tree, arr_data1+i, f_compare);
        avl_print(tree, f_print, 4);
    }
    avl_destroy(tree);
}

void t_insert() {
    printf("*************** TEST (INSERT) ***************\n");
    avl_tree *tree = avl_create();
    int arr_data1[] = {1, 2, 3, 4, 5, 6, 7};
    for (int i = 0; i < LEN(arr_data1); i++) {
        printf("Inserting: %d\n", arr_data1[i]);
        avl_insert(tree, arr_data1+i, f_compare);
        avl_print(tree, f_print, 4);
    }
    avl_delete_all(tree);
    printf("\n!!!!!!!!!!!!!!!! All deleted. !!!!!!!!!!!!!!!!\n\n");
    int arr_data2[] = {7, 6, 5, 4, 3, 2, 1};
    for (int i = 0; i < LEN(arr_data2); i++) {
        printf("Inserting: %d\n", arr_data2[i]);
        avl_insert(tree, arr_data2+i, f_compare);
        avl_print(tree, f_print, 4);
    }
    avl_delete_all(tree);
    printf("\n!!!!!!!!!!!!!!!! All deleted. !!!!!!!!!!!!!!!!\n\n");
    int arr_data3[] = {1, 3, 2};
    for (int i = 0; i < LEN(arr_data3); i++) {
        printf("Inserting: %d\n", arr_data3[i]);
        avl_insert(tree, arr_data3+i, f_compare);
        avl_print(tree, f_print, 4);
    }
    avl_delete_all(tree);
    printf("\n!!!!!!!!!!!!!!!! All deleted. !!!!!!!!!!!!!!!!\n\n");
    int arr_data4[] = {2, 1, 3};
    for (int i = 0; i < LEN(arr_data4); i++) {
        printf("Inserting: %d\n", arr_data4[i]);
        avl_insert(tree, arr_data4+i, f_compare);
        avl_print(tree, f_print, 4);
    }
    avl_delete_all(tree);
    printf("\n!!!!!!!!!!!!!!!! All deleted. !!!!!!!!!!!!!!!!\n\n");
    int arr_data5[] = {2, 1, 5, 3, 6, 4};
    for (int i = 0; i < LEN(arr_data5); i++) {
        printf("Inserting: %d\n", arr_data5[i]);
        avl_insert(tree, arr_data5+i, f_compare);
        avl_print(tree, f_print, 4);
    }
    avl_delete_all(tree);
    printf("\n!!!!!!!!!!!!!!!! All deleted. !!!!!!!!!!!!!!!!\n\n");
    int arr_data6[] = {2, 1, 5, 4, 6, 3};
    for (int i = 0; i < LEN(arr_data6); i++) {
        printf("Inserting: %d\n", arr_data6[i]);
        avl_insert(tree, arr_data6+i, f_compare);
        avl_print(tree, f_print, 4);
    }
    avl_destroy(tree);
}

unsigned char f_compare(void *new_data, void *old_data) {
    return *((int *) new_data) < *((int *) old_data) ? 1 : 0;
}

void f_print(void *data) {
    printf("[%2d]", *((int *) data));
}

#endif