// Data structures demo in one file.
// Keep this file self-contained and focused on basic usage.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -----------------------------
// Sequential List (fixed array)
// -----------------------------
typedef struct {
	int data[16];
	int size;
} SeqList;

void seqlist_init(SeqList *list) {
	list->size = 0;
}

int seqlist_insert(SeqList *list, int index, int value) {
	if (index < 0 || index > list->size || list->size >= 16) {
		return 0;
	}
	for (int i = list->size; i > index; --i) {
		list->data[i] = list->data[i - 1];
	}
	list->data[index] = value;
	list->size++;
	return 1;
}

int seqlist_remove(SeqList *list, int index) {
	if (index < 0 || index >= list->size) {
		return 0;
	}
	for (int i = index; i < list->size - 1; ++i) {
		list->data[i] = list->data[i + 1];
	}
	list->size--;
	return 1;
}

int seqlist_find(const SeqList *list, int value) {
	for (int i = 0; i < list->size; ++i) {
		if (list->data[i] == value) {
			return i;
		}
	}
	return -1;
}

void seqlist_print(const SeqList *list) {
	printf("SeqList: ");
	for (int i = 0; i < list->size; ++i) {
		printf("%d ", list->data[i]);
	}
	printf("\n");
}

// -----------------------------
// Dynamic Array
// -----------------------------
typedef struct {
	int *data;
	int size;
	int capacity;
} DynArray;

void dynarray_init(DynArray *arr, int capacity) {
	arr->data = (int *)malloc(sizeof(int) * capacity);
	arr->size = 0;
	arr->capacity = capacity;
}

void dynarray_free(DynArray *arr) {
	free(arr->data);
	arr->data = NULL;
	arr->size = 0;
	arr->capacity = 0;
}

static void dynarray_resize(DynArray *arr, int new_capacity) {
	int *new_data = (int *)realloc(arr->data, sizeof(int) * new_capacity);
	if (new_data == NULL) {
		return; // keep old buffer on failure
	}
	arr->data = new_data;
	arr->capacity = new_capacity;
}

void dynarray_push_back(DynArray *arr, int value) {
	if (arr->size == arr->capacity) {
		dynarray_resize(arr, arr->capacity * 2);
	}
	arr->data[arr->size++] = value;
}

int dynarray_insert(DynArray *arr, int index, int value) {
	if (index < 0 || index > arr->size) {
		return 0;
	}
	if (arr->size == arr->capacity) {
		dynarray_resize(arr, arr->capacity * 2);
	}
	for (int i = arr->size; i > index; --i) {
		arr->data[i] = arr->data[i - 1];
	}
	arr->data[index] = value;
	arr->size++;
	return 1;
}

int dynarray_remove(DynArray *arr, int index) {
	if (index < 0 || index >= arr->size) {
		return 0;
	}
	for (int i = index; i < arr->size - 1; ++i) {
		arr->data[i] = arr->data[i + 1];
	}
	arr->size--;
	return 1;
}

void dynarray_print(const DynArray *arr) {
	printf("DynArray: ");
	for (int i = 0; i < arr->size; ++i) {
		printf("%d ", arr->data[i]);
	}
	printf("\n");
}

// -----------------------------
// Doubly Linked List
// -----------------------------
typedef struct DNode {
	int value;
	struct DNode *prev;
	struct DNode *next;
} DNode;

typedef struct {
	DNode *head;
	DNode *tail;
} DList;

void dlist_init(DList *list) {
	list->head = NULL;
	list->tail = NULL;
}

void dlist_push_back(DList *list, int value) {
	DNode *node = (DNode *)malloc(sizeof(DNode));
	node->value = value;
	node->next = NULL;
	node->prev = list->tail;
	if (list->tail) {
		list->tail->next = node;
	} else {
		list->head = node;
	}
	list->tail = node;
}

void dlist_push_front(DList *list, int value) {
	DNode *node = (DNode *)malloc(sizeof(DNode));
	node->value = value;
	node->prev = NULL;
	node->next = list->head;
	if (list->head) {
		list->head->prev = node;
	} else {
		list->tail = node;
	}
	list->head = node;
}

int dlist_remove_value(DList *list, int value) {
	DNode *cur = list->head;
	while (cur) {
		if (cur->value == value) {
			if (cur->prev) {
				cur->prev->next = cur->next;
			} else {
				list->head = cur->next;
			}
			if (cur->next) {
				cur->next->prev = cur->prev;
			} else {
				list->tail = cur->prev;
			}
			free(cur);
			return 1;
		}
		cur = cur->next;
	}
	return 0;
}

void dlist_print_forward(const DList *list) {
	printf("DList forward: ");
	for (DNode *cur = list->head; cur; cur = cur->next) {
		printf("%d ", cur->value);
	}
	printf("\n");
}

void dlist_print_backward(const DList *list) {
	printf("DList backward: ");
	for (DNode *cur = list->tail; cur; cur = cur->prev) {
		printf("%d ", cur->value);
	}
	printf("\n");
}

void dlist_clear(DList *list) {
	DNode *cur = list->head;
	while (cur) {
		DNode *next = cur->next;
		free(cur);
		cur = next;
	}
	list->head = NULL;
	list->tail = NULL;
}

// -----------------------------
// Circular Linked List (singly)
// -----------------------------
typedef struct CNode {
	int value;
	struct CNode *next;
} CNode;

typedef struct {
	CNode *tail; // tail->next is head
} CList;

void clist_init(CList *list) {
	list->tail = NULL;
}

void clist_push_back(CList *list, int value) {
	CNode *node = (CNode *)malloc(sizeof(CNode));
	node->value = value;
	if (list->tail == NULL) {
		node->next = node;
		list->tail = node;
		return;
	}
	node->next = list->tail->next;
	list->tail->next = node;
	list->tail = node;
}

int clist_remove_value(CList *list, int value) {
	if (list->tail == NULL) {
		return 0;
	}
	CNode *prev = list->tail;
	CNode *cur = list->tail->next;
	do {
		if (cur->value == value) {
			prev->next = cur->next;
			if (cur == list->tail) {
				if (cur->next == cur) {
					list->tail = NULL;
				} else {
					list->tail = prev;
				}
			}
			free(cur);
			return 1;
		}
		prev = cur;
		cur = cur->next;
	} while (cur != list->tail->next);
	return 0;
}

void clist_print_once(const CList *list) {
	printf("CList: ");
	if (list->tail == NULL) {
		printf("(empty)\n");
		return;
	}
	CNode *cur = list->tail->next;
	do {
		printf("%d ", cur->value);
		cur = cur->next;
	} while (cur != list->tail->next);
	printf("\n");
}

void clist_clear(CList *list) {
	if (list->tail == NULL) {
		return;
	}
	CNode *cur = list->tail->next;
	while (cur != list->tail) {
		CNode *next = cur->next;
		free(cur);
		cur = next;
	}
	free(list->tail);
	list->tail = NULL;
}

// -----------------------------
// Stack (array)
// -----------------------------
typedef struct {
	int data[16];
	int top;
} Stack;

void stack_init(Stack *s) {
	s->top = 0;
}

int stack_push(Stack *s, int value) {
	if (s->top >= 16) {
		return 0;
	}
	s->data[s->top++] = value;
	return 1;
}

int stack_pop(Stack *s, int *out) {
	if (s->top == 0) {
		return 0;
	}
	*out = s->data[--s->top];
	return 1;
}

int stack_peek(const Stack *s, int *out) {
	if (s->top == 0) {
		return 0;
	}
	*out = s->data[s->top - 1];
	return 1;
}

// -----------------------------
// Queue (circular array)
// -----------------------------
typedef struct {
	int data[16];
	int head;
	int tail;
	int size;
} Queue;

void queue_init(Queue *q) {
	q->head = 0;
	q->tail = 0;
	q->size = 0;
}

int queue_enqueue(Queue *q, int value) {
	if (q->size >= 16) {
		return 0;
	}
	q->data[q->tail] = value;
	q->tail = (q->tail + 1) % 16;
	q->size++;
	return 1;
}

int queue_dequeue(Queue *q, int *out) {
	if (q->size == 0) {
		return 0;
	}
	*out = q->data[q->head];
	q->head = (q->head + 1) % 16;
	q->size--;
	return 1;
}

int queue_front(const Queue *q, int *out) {
	if (q->size == 0) {
		return 0;
	}
	*out = q->data[q->head];
	return 1;
}

// -----------------------------
// Binary Tree (general)
// -----------------------------
typedef struct BNode {
	int value;
	struct BNode *left;
	struct BNode *right;
} BNode;

BNode *bnode_create(int value) {
	BNode *node = (BNode *)malloc(sizeof(BNode));
	node->value = value;
	node->left = NULL;
	node->right = NULL;
	return node;
}

void btree_preorder(const BNode *root) {
	if (!root) return;
	printf("%d ", root->value);
	btree_preorder(root->left);
	btree_preorder(root->right);
}

void btree_inorder(const BNode *root) {
	if (!root) return;
	btree_inorder(root->left);
	printf("%d ", root->value);
	btree_inorder(root->right);
}

void btree_postorder(const BNode *root) {
	if (!root) return;
	btree_postorder(root->left);
	btree_postorder(root->right);
	printf("%d ", root->value);
}

int btree_height(const BNode *root) {
	if (!root) return 0;
	int hl = btree_height(root->left);
	int hr = btree_height(root->right);
	return (hl > hr ? hl : hr) + 1;
}

void btree_free(BNode *root) {
	if (!root) return;
	btree_free(root->left);
	btree_free(root->right);
	free(root);
}

// -----------------------------
// Binary Search Tree (BST)
// -----------------------------
BNode *bst_insert(BNode *root, int value) {
	if (!root) return bnode_create(value);
	if (value < root->value) {
		root->left = bst_insert(root->left, value);
	} else if (value > root->value) {
		root->right = bst_insert(root->right, value);
	}
	return root;
}

const BNode *bst_search(const BNode *root, int value) {
	if (!root) return NULL;
	if (value == root->value) return root;
	if (value < root->value) return bst_search(root->left, value);
	return bst_search(root->right, value);
}

static BNode *bst_find_min(BNode *root) {
	while (root && root->left) {
		root = root->left;
	}
	return root;
}

BNode *bst_remove(BNode *root, int value) {
	if (!root) return NULL;
	if (value < root->value) {
		root->left = bst_remove(root->left, value);
	} else if (value > root->value) {
		root->right = bst_remove(root->right, value);
	} else {
		if (!root->left) {
			BNode *right = root->right;
			free(root);
			return right;
		}
		if (!root->right) {
			BNode *left = root->left;
			free(root);
			return left;
		}
		BNode *min = bst_find_min(root->right);
		root->value = min->value;
		root->right = bst_remove(root->right, min->value);
	}
	return root;
}

// -----------------------------
// AVL Tree (balanced BST)
// -----------------------------
typedef struct AVLNode {
	int value;
	int height;
	struct AVLNode *left;
	struct AVLNode *right;
} AVLNode;

static int avl_height(const AVLNode *node) {
	return node ? node->height : 0;
}

static int avl_max(int a, int b) {
	return a > b ? a : b;
}

static AVLNode *avl_create(int value) {
	AVLNode *node = (AVLNode *)malloc(sizeof(AVLNode));
	node->value = value;
	node->height = 1;
	node->left = NULL;
	node->right = NULL;
	return node;
}

static AVLNode *avl_rotate_right(AVLNode *y) {
	AVLNode *x = y->left;
	AVLNode *t2 = x->right;
	x->right = y;
	y->left = t2;
	y->height = avl_max(avl_height(y->left), avl_height(y->right)) + 1;
	x->height = avl_max(avl_height(x->left), avl_height(x->right)) + 1;
	return x;
}

static AVLNode *avl_rotate_left(AVLNode *x) {
	AVLNode *y = x->right;
	AVLNode *t2 = y->left;
	y->left = x;
	x->right = t2;
	x->height = avl_max(avl_height(x->left), avl_height(x->right)) + 1;
	y->height = avl_max(avl_height(y->left), avl_height(y->right)) + 1;
	return y;
}

static int avl_balance(const AVLNode *node) {
	return node ? avl_height(node->left) - avl_height(node->right) : 0;
}

AVLNode *avl_insert(AVLNode *node, int value) {
	if (!node) return avl_create(value);
	if (value < node->value) {
		node->left = avl_insert(node->left, value);
	} else if (value > node->value) {
		node->right = avl_insert(node->right, value);
	} else {
		return node; // no duplicates
	}
	node->height = 1 + avl_max(avl_height(node->left), avl_height(node->right));
	int balance = avl_balance(node);

	if (balance > 1 && value < node->left->value) {
		return avl_rotate_right(node);
	}
	if (balance < -1 && value > node->right->value) {
		return avl_rotate_left(node);
	}
	if (balance > 1 && value > node->left->value) {
		node->left = avl_rotate_left(node->left);
		return avl_rotate_right(node);
	}
	if (balance < -1 && value < node->right->value) {
		node->right = avl_rotate_right(node->right);
		return avl_rotate_left(node);
	}
	return node;
}

void avl_inorder(const AVLNode *root) {
	if (!root) return;
	avl_inorder(root->left);
	printf("%d ", root->value);
	avl_inorder(root->right);
}

void avl_free(AVLNode *root) {
	if (!root) return;
	avl_free(root->left);
	avl_free(root->right);
	free(root);
}

// -----------------------------
// Heap (max-heap)
// -----------------------------
typedef struct {
	int data[32];
	int size;
} Heap;

void heap_init(Heap *h) {
	h->size = 0;
}

static void heap_swap(int *a, int *b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void heap_push(Heap *h, int value) {
	if (h->size >= 32) return;
	int i = h->size++;
	h->data[i] = value;
	while (i > 0) {
		int parent = (i - 1) / 2;
		if (h->data[parent] >= h->data[i]) break;
		heap_swap(&h->data[parent], &h->data[i]);
		i = parent;
	}
}

int heap_pop(Heap *h, int *out) {
	if (h->size == 0) return 0;
	*out = h->data[0];
	h->data[0] = h->data[--h->size];
	int i = 0;
	while (1) {
		int left = i * 2 + 1;
		int right = i * 2 + 2;
		int largest = i;
		if (left < h->size && h->data[left] > h->data[largest]) largest = left;
		if (right < h->size && h->data[right] > h->data[largest]) largest = right;
		if (largest == i) break;
		heap_swap(&h->data[i], &h->data[largest]);
		i = largest;
	}
	return 1;
}

// -----------------------------
// Graph (adjacency list)
// -----------------------------
typedef struct GNode {
	int to;
	struct GNode *next;
} GNode;

typedef struct {
	int n;
	GNode *adj[8];
} Graph;

void graph_init(Graph *g, int n) {
	g->n = n;
	for (int i = 0; i < 8; ++i) {
		g->adj[i] = NULL;
	}
}

static void graph_add_edge_oneway(Graph *g, int u, int v) {
	GNode *node = (GNode *)malloc(sizeof(GNode));
	node->to = v;
	node->next = g->adj[u];
	g->adj[u] = node;
}

void graph_add_undirected(Graph *g, int u, int v) {
	graph_add_edge_oneway(g, u, v);
	graph_add_edge_oneway(g, v, u);
}

void graph_add_directed(Graph *g, int u, int v) {
	graph_add_edge_oneway(g, u, v);
}

void graph_dfs(const Graph *g, int start, int *visited) {
	visited[start] = 1;
	printf("%d ", start);
	for (GNode *cur = g->adj[start]; cur; cur = cur->next) {
		if (!visited[cur->to]) {
			graph_dfs(g, cur->to, visited);
		}
	}
}

void graph_bfs(const Graph *g, int start) {
	int visited[8] = {0};
	int queue[8];
	int head = 0;
	int tail = 0;
	visited[start] = 1;
	queue[tail++] = start;
	while (head < tail) {
		int u = queue[head++];
		printf("%d ", u);
		for (GNode *cur = g->adj[u]; cur; cur = cur->next) {
			if (!visited[cur->to]) {
				visited[cur->to] = 1;
				queue[tail++] = cur->to;
			}
		}
	}
}

void graph_free(Graph *g) {
	for (int i = 0; i < g->n; ++i) {
		GNode *cur = g->adj[i];
		while (cur) {
			GNode *next = cur->next;
			free(cur);
			cur = next;
		}
		g->adj[i] = NULL;
	}
}

// -----------------------------
// Hash Table (open addressing)
// -----------------------------
typedef struct {
	int key;
	int value;
	int state; // 0 empty, 1 used, 2 deleted
} HashEntry;

typedef struct {
	HashEntry table[23];
	int size;
} HashTable;

void hashtable_init(HashTable *ht) {
	memset(ht, 0, sizeof(HashTable));
}

static int hashtable_hash(int key) {
	return (key * 2654435761u) % 23;
}

int hashtable_put(HashTable *ht, int key, int value) {
	if (ht->size >= 17) {
		return 0; // keep load factor moderate
	}
	int idx = hashtable_hash(key);
	for (int i = 0; i < 23; ++i) {
		int pos = (idx + i) % 23;
		if (ht->table[pos].state != 1) {
			ht->table[pos].key = key;
			ht->table[pos].value = value;
			ht->table[pos].state = 1;
			ht->size++;
			return 1;
		}
		if (ht->table[pos].state == 1 && ht->table[pos].key == key) {
			ht->table[pos].value = value;
			return 1;
		}
	}
	return 0;
}

int hashtable_get(const HashTable *ht, int key, int *out) {
	int idx = hashtable_hash(key);
	for (int i = 0; i < 23; ++i) {
		int pos = (idx + i) % 23;
		if (ht->table[pos].state == 0) {
			return 0;
		}
		if (ht->table[pos].state == 1 && ht->table[pos].key == key) {
			*out = ht->table[pos].value;
			return 1;
		}
	}
	return 0;
}

int hashtable_remove(HashTable *ht, int key) {
	int idx = hashtable_hash(key);
	for (int i = 0; i < 23; ++i) {
		int pos = (idx + i) % 23;
		if (ht->table[pos].state == 0) {
			return 0;
		}
		if (ht->table[pos].state == 1 && ht->table[pos].key == key) {
			ht->table[pos].state = 2;
			ht->size--;
			return 1;
		}
	}
	return 0;
}

// -----------------------------
// Demo in main
// -----------------------------
int main(void) {
	// Sequential list
	SeqList sl;
	seqlist_init(&sl);
	seqlist_insert(&sl, 0, 10);
	seqlist_insert(&sl, 1, 20);
	seqlist_insert(&sl, 1, 15);
	seqlist_print(&sl);
	seqlist_remove(&sl, 1);
	printf("SeqList find 20 at %d\n", seqlist_find(&sl, 20));

	// Dynamic array
	DynArray da;
	dynarray_init(&da, 4);
	dynarray_push_back(&da, 1);
	dynarray_push_back(&da, 2);
	dynarray_insert(&da, 1, 99);
	dynarray_remove(&da, 0);
	dynarray_print(&da);
	dynarray_free(&da);

	// Doubly linked list
	DList dl;
	dlist_init(&dl);
	dlist_push_back(&dl, 3);
	dlist_push_front(&dl, 2);
	dlist_push_back(&dl, 4);
	dlist_print_forward(&dl);
	dlist_print_backward(&dl);
	dlist_remove_value(&dl, 3);
	dlist_print_forward(&dl);
	dlist_clear(&dl);

	// Circular linked list
	CList cl;
	clist_init(&cl);
	clist_push_back(&cl, 7);
	clist_push_back(&cl, 8);
	clist_push_back(&cl, 9);
	clist_print_once(&cl);
	clist_remove_value(&cl, 8);
	clist_print_once(&cl);
	clist_clear(&cl);

	// Stack
	Stack st;
	stack_init(&st);
	stack_push(&st, 11);
	stack_push(&st, 22);
	int top;
	if (stack_peek(&st, &top)) {
		printf("Stack top: %d\n", top);
	}
	while (stack_pop(&st, &top)) {
		printf("Stack pop: %d\n", top);
	}

	// Queue
	Queue q;
	queue_init(&q);
	queue_enqueue(&q, 5);
	queue_enqueue(&q, 6);
	int front;
	if (queue_front(&q, &front)) {
		printf("Queue front: %d\n", front);
	}
	while (queue_dequeue(&q, &front)) {
		printf("Queue dequeue: %d\n", front);
	}

	// Binary tree
	BNode *bt = bnode_create(1);
	bt->left = bnode_create(2);
	bt->right = bnode_create(3);
	bt->left->left = bnode_create(4);
	bt->left->right = bnode_create(5);
	printf("BT preorder: ");
	btree_preorder(bt);
	printf("\nBT inorder: ");
	btree_inorder(bt);
	printf("\nBT postorder: ");
	btree_postorder(bt);
	printf("\nBT height: %d\n", btree_height(bt));
	btree_free(bt);

	// BST
	BNode *bst = NULL;
	bst = bst_insert(bst, 8);
	bst = bst_insert(bst, 3);
	bst = bst_insert(bst, 10);
	bst = bst_insert(bst, 1);
	bst = bst_insert(bst, 6);
	printf("BST inorder: ");
	btree_inorder(bst);
	printf("\nBST search 6: %s\n", bst_search(bst, 6) ? "found" : "not found");
	bst = bst_remove(bst, 3);
	printf("BST inorder after remove: ");
	btree_inorder(bst);
	printf("\n");
	btree_free(bst);

	// AVL tree
	AVLNode *avl = NULL;
	int avl_values[] = {10, 20, 30, 40, 50, 25};
	for (int i = 0; i < 6; ++i) {
		avl = avl_insert(avl, avl_values[i]);
	}
	printf("AVL inorder: ");
	avl_inorder(avl);
	printf("\n");
	avl_free(avl);

	// Heap
	Heap heap;
	heap_init(&heap);
	heap_push(&heap, 4);
	heap_push(&heap, 15);
	heap_push(&heap, 7);
	int max;
	if (heap_pop(&heap, &max)) {
		printf("Heap pop: %d\n", max);
	}

	// Undirected graph
	Graph ug;
	graph_init(&ug, 5);
	graph_add_undirected(&ug, 0, 1);
	graph_add_undirected(&ug, 0, 2);
	graph_add_undirected(&ug, 1, 3);
	graph_add_undirected(&ug, 2, 4);
	int visited_ug[8] = {0};
	printf("UG DFS: ");
	graph_dfs(&ug, 0, visited_ug);
	printf("\nUG BFS: ");
	graph_bfs(&ug, 0);
	printf("\n");
	graph_free(&ug);

	// Directed graph
	Graph dg;
	graph_init(&dg, 5);
	graph_add_directed(&dg, 0, 1);
	graph_add_directed(&dg, 0, 2);
	graph_add_directed(&dg, 1, 3);
	graph_add_directed(&dg, 2, 3);
	graph_add_directed(&dg, 3, 4);
	int visited_dg[8] = {0};
	printf("DG DFS: ");
	graph_dfs(&dg, 0, visited_dg);
	printf("\nDG BFS: ");
	graph_bfs(&dg, 0);
	printf("\n");
	graph_free(&dg);

	// Hash table
	HashTable ht;
	hashtable_init(&ht);
	hashtable_put(&ht, 42, 100);
	hashtable_put(&ht, 7, 200);
	int value;
	if (hashtable_get(&ht, 42, &value)) {
		printf("Hash get 42: %d\n", value);
	}
	hashtable_remove(&ht, 7);
	printf("Hash get 7: %s\n", hashtable_get(&ht, 7, &value) ? "found" : "not found");

	return 0;
}
