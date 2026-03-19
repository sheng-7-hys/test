// 数据结构综合演示 - 单文件实现
// 包含顺序表、动态数组、双向链表、循环链表、栈、队列、二叉树、BST、AVL、堆、图、哈希表等基础数据结构
// 保持代码自包含，聚焦基础用法演示

#include <stdio.h>   // 标准输入输出
#include <stdlib.h>  // 内存分配/释放、标准库函数
#include <string.h>  // 内存操作函数（如memset）

// -----------------------------
// 顺序表（固定数组实现）- 底层是连续内存，大小固定
// -----------------------------
// 顺序表结构体定义：固定大小数组+有效元素个数
typedef struct {
    int data[16];  // 存储元素的数组，最大容量16
    int size;      // 有效元素个数（当前长度）
} SeqList;

// 初始化顺序表：清空有效元素个数
void seqlist_init(SeqList *list) {
    list->size = 0;  // 初始化为空表
}

// 顺序表插入元素
// 参数：list-顺序表指针，index-插入位置（0~size），value-插入值
// 返回：1成功，0失败（位置非法/表满）
int seqlist_insert(SeqList *list, int index, int value) {
    // 边界检查：位置越界 或 表已满（容量16）
    if (index < 0 || index > list->size || list->size >= 16) {
        return 0;
    }
    // 从后往前移动元素，腾出插入位置
    for (int i = list->size; i > index; --i) {
        list->data[i] = list->data[i - 1];
    }
    list->data[index] = value;  // 插入新值
    list->size++;               // 有效元素数+1
    return 1;
}

// 顺序表删除指定位置元素
// 参数：list-顺序表指针，index-删除位置（0~size-1）
// 返回：1成功，0失败（位置非法）
int seqlist_remove(SeqList *list, int index) {
    // 边界检查：位置越界
    if (index < 0 || index >= list->size) {
        return 0;
    }
    // 从删除位置往后覆盖，前移元素
    for (int i = index; i < list->size - 1; ++i) {
        list->data[i] = list->data[i + 1];
    }
    list->size--;  // 有效元素数-1
    return 1;
}

// 顺序表查找指定值的位置
// 参数：list-顺序表指针（只读），value-查找值
// 返回：找到返回索引，未找到返回-1
int seqlist_find(const SeqList *list, int value) {
    // 遍历有效元素，线性查找
    for (int i = 0; i < list->size; ++i) {
        if (list->data[i] == value) {
            return i;  // 找到返回索引
        }
    }
    return -1;  // 未找到
}

// 打印顺序表所有元素
void seqlist_print(const SeqList *list) {
    printf("SeqList: ");
    for (int i = 0; i < list->size; ++i) {
        printf("%d ", list->data[i]);
    }
    printf("\n");
}

// -----------------------------
// 动态数组 - 底层数组可自动扩容，解决顺序表容量固定问题
// -----------------------------
// 动态数组结构体定义：动态内存数组+有效元素数+当前容量
typedef struct {
    int *data;     // 动态分配的数组指针
    int size;      // 有效元素个数
    int capacity;  // 当前数组容量（可容纳的最大元素数）
} DynArray;

// 初始化动态数组
// 参数：arr-动态数组指针，capacity-初始容量
void dynarray_init(DynArray *arr, int capacity) {
    // 分配初始容量的内存
    arr->data = (int *)malloc(sizeof(int) * capacity);
    arr->size = 0;         // 初始有效元素数0
    arr->capacity = capacity;  // 记录初始容量
}

// 释放动态数组内存（防止内存泄漏）
void dynarray_free(DynArray *arr) {
    free(arr->data);       // 释放底层数组
    arr->data = NULL;      // 置空指针，避免野指针
    arr->size = 0;         // 重置状态
    arr->capacity = 0;
}

// 动态扩容（内部函数，仅当前文件可见）
// 参数：arr-动态数组指针，new_capacity-新容量
static void dynarray_resize(DynArray *arr, int new_capacity) {
    // 重新分配内存（保留原有数据）
    int *new_data = (int *)realloc(arr->data, sizeof(int) * new_capacity);
    if (new_data == NULL) {
        return;  // 扩容失败，保留原内存
    }
    arr->data = new_data;       // 更新数组指针
    arr->capacity = new_capacity;  // 更新容量
}

// 尾插元素（简化版插入）
// 参数：arr-动态数组指针，value-插入值
void dynarray_push_back(DynArray *arr, int value) {
    // 容量满时，扩容为原来的2倍（倍增策略）
    if (arr->size == arr->capacity) {
        dynarray_resize(arr, arr->capacity * 2);
    }
    arr->data[arr->size++] = value;  // 尾插+有效元素数+1
}

// 动态数组指定位置插入元素
// 参数：arr-动态数组指针，index-插入位置（0~size），value-插入值
// 返回：1成功，0失败（位置非法）
int dynarray_insert(DynArray *arr, int index, int value) {
    // 边界检查：位置越界
    if (index < 0 || index > arr->size) {
        return 0;
    }
    // 容量满时扩容
    if (arr->size == arr->capacity) {
        dynarray_resize(arr, arr->capacity * 2);
    }
    // 后移元素腾出位置
    for (int i = arr->size; i > index; --i) {
        arr->data[i] = arr->data[i - 1];
    }
    arr->data[index] = value;  // 插入新值
    arr->size++;               // 有效元素数+1
    return 1;
}

// 动态数组删除指定位置元素
// 参数：arr-动态数组指针，index-删除位置（0~size-1）
// 返回：1成功，0失败（位置非法）
int dynarray_remove(DynArray *arr, int index) {
    // 边界检查：位置越界
    if (index < 0 || index >= arr->size) {
        return 0;
    }
    // 前移元素覆盖删除位置
    for (int i = index; i < arr->size - 1; ++i) {
        arr->data[i] = arr->data[i + 1];
    }
    arr->size--;  // 有效元素数-1
    return 1;
}

// 打印动态数组所有元素
void dynarray_print(const DynArray *arr) {
    printf("DynArray: ");
    for (int i = 0; i < arr->size; ++i) {
        printf("%d ", arr->data[i]);
    }
    printf("\n");
}

// -----------------------------
// 双向链表 - 每个节点含前驱/后继指针，支持前后遍历、增删
// -----------------------------
// 双向链表节点结构体
typedef struct DNode {
    int value;          // 节点值
    struct DNode *prev; // 前驱节点指针
    struct DNode *next; // 后继节点指针
} DNode;

// 双向链表结构体（头/尾指针，简化操作）
typedef struct {
    DNode *head; // 头节点指针
    DNode *tail; // 尾节点指针
} DList;

// 初始化双向链表（空表）
void dlist_init(DList *list) {
    list->head = NULL;
    list->tail = NULL;
}

// 双向链表尾插元素
// 参数：list-链表指针，value-插入值
void dlist_push_back(DList *list, int value) {
    // 分配新节点内存
    DNode *node = (DNode *)malloc(sizeof(DNode));
    node->value = value;  // 赋值
    node->next = NULL;    // 尾节点后继为NULL
    node->prev = list->tail;  // 新节点前驱指向原尾节点

    if (list->tail) {          // 原链表非空
        list->tail->next = node; // 原尾节点后继指向新节点
    } else {                   // 原链表为空
        list->head = node;      // 新节点作为头节点
    }
    list->tail = node;  // 更新尾节点为新节点
}

// 双向链表头插元素
// 参数：list-链表指针，value-插入值
void dlist_push_front(DList *list, int value) {
    // 分配新节点内存
    DNode *node = (DNode *)malloc(sizeof(DNode));
    node->value = value;   // 赋值
    node->prev = NULL;     // 头节点前驱为NULL
    node->next = list->head; // 新节点后继指向原头节点

    if (list->head) {          // 原链表非空
        list->head->prev = node; // 原头节点前驱指向新节点
    } else {                   // 原链表为空
        list->tail = node;      // 新节点作为尾节点
    }
    list->head = node;  // 更新头节点为新节点
}

// 双向链表删除第一个值为value的节点
// 参数：list-链表指针，value-要删除的值
// 返回：1成功，0失败（未找到）
int dlist_remove_value(DList *list, int value) {
    // 遍历链表找目标节点
    DNode *cur = list->head;
    while (cur) {
        if (cur->value == value) { // 找到目标节点
            // 处理前驱节点：若不是头节点，前驱后继指向当前节点的后继
            if (cur->prev) {
                cur->prev->next = cur->next;
            } else { // 是头节点，更新头节点为当前节点的后继
                list->head = cur->next;
            }
            // 处理后继节点：若不是尾节点，后继前驱指向当前节点的前驱
            if (cur->next) {
                cur->next->prev = cur->prev;
            } else { // 是尾节点，更新尾节点为当前节点的前驱
                list->tail = cur->prev;
            }
            free(cur); // 释放节点内存
            return 1;  // 删除成功
        }
        cur = cur->next; // 遍历下一个节点
    }
    return 0; // 未找到目标值
}

// 正向打印双向链表（头→尾）
void dlist_print_forward(const DList *list) {
    printf("DList forward: ");
    for (DNode *cur = list->head; cur; cur = cur->next) {
        printf("%d ", cur->value);
    }
    printf("\n");
}

// 反向打印双向链表（尾→头）
void dlist_print_backward(const DList *list) {
    printf("DList backward: ");
    for (DNode *cur = list->tail; cur; cur = cur->prev) {
        printf("%d ", cur->value);
    }
    printf("\n");
}

// 清空双向链表（释放所有节点内存）
void dlist_clear(DList *list) {
    DNode *cur = list->head;
    while (cur) {
        DNode *next = cur->next; // 先保存后继节点
        free(cur);               // 释放当前节点
        cur = next;              // 遍历下一个
    }
    // 重置链表状态
    list->head = NULL;
    list->tail = NULL;
}

// -----------------------------
// 循环单链表 - 尾节点后继指向头节点，无NULL指针
// -----------------------------
// 循环链表节点结构体
typedef struct CNode {
    int value;         // 节点值
    struct CNode *next;// 后继节点指针（尾节点→头节点）
} CNode;

// 循环链表结构体（仅存尾节点：tail->next = 头节点，简化操作）
typedef struct {
    CNode *tail; // 尾节点指针
} CList;

// 初始化循环链表（空表）
void clist_init(CList *list) {
    list->tail = NULL;
}

// 循环链表尾插元素
// 参数：list-链表指针，value-插入值
void clist_push_back(CList *list, int value) {
    // 分配新节点
    CNode *node = (CNode *)malloc(sizeof(CNode));
    node->value = value;

    if (list->tail == NULL) { // 空表
        node->next = node;    // 自环（唯一节点）
        list->tail = node;    // 尾节点指向自己
        return;
    }
    // 非空表：新节点后继指向原头节点（tail->next）
    node->next = list->tail->next;
    list->tail->next = node;  // 原尾节点后继指向新节点
    list->tail = node;        // 更新尾节点为新节点
}

// 循环链表删除第一个值为value的节点
// 参数：list-链表指针，value-要删除的值
// 返回：1成功，0失败（未找到/空表）
int clist_remove_value(CList *list, int value) {
    if (list->tail == NULL) { // 空表直接返回
        return 0;
    }

    CNode *prev = list->tail;    // 前驱节点（初始为尾节点）
    CNode *cur = list->tail->next; // 当前节点（初始为头节点）

    // 遍历循环链表（回到头节点则结束）
    do {
        if (cur->value == value) { // 找到目标节点
            prev->next = cur->next; // 前驱节点跳过当前节点

            // 处理尾节点：若删除的是尾节点
            if (cur == list->tail) {
                if (cur->next == cur) { // 链表只剩一个节点
                    list->tail = NULL;  // 置空
                } else {                // 链表有多个节点
                    list->tail = prev;  // 更新尾节点为前驱
                }
            }
            free(cur); // 释放节点内存
            return 1;  // 删除成功
        }
        prev = cur;    // 前驱后移
        cur = cur->next; // 当前节点后移
    } while (cur != list->tail->next); // 遍历一圈结束

    return 0; // 未找到目标值
}

// 打印循环链表一次（避免无限循环）
void clist_print_once(const CList *list) {
    printf("CList: ");
    if (list->tail == NULL) { // 空表
        printf("(empty)\n");
        return;
    }
    CNode *cur = list->tail->next; // 从头节点开始
    do {
        printf("%d ", cur->value);
        cur = cur->next;
    } while (cur != list->tail->next); // 回到头节点结束
    printf("\n");
}

// 清空循环链表（释放所有节点内存）
void clist_clear(CList *list) {
    if (list->tail == NULL) { // 空表直接返回
        return;
    }
    CNode *cur = list->tail->next; // 从头节点开始
    // 遍历到尾节点前一个
    while (cur != list->tail) {
        CNode *next = cur->next; // 保存后继
        free(cur);               // 释放当前
        cur = next;              // 后移
    }
    free(list->tail); // 释放尾节点
    list->tail = NULL; // 重置状态
}

// -----------------------------
// 栈（数组实现）- 先进后出（FILO）
// -----------------------------
// 栈结构体：固定数组+栈顶指针（top指向栈顶下一个位置）
typedef struct {
    int data[16]; // 栈空间，容量16
    int top;      // 栈顶指针（0=空栈，16=满栈）
} Stack;

// 初始化栈（空栈）
void stack_init(Stack *s) {
    s->top = 0; // 栈顶指针置0
}

// 栈压入元素（入栈）
// 参数：s-栈指针，value-入栈值
// 返回：1成功，0失败（栈满）
int stack_push(Stack *s, int value) {
    if (s->top >= 16) { // 栈满
        return 0;
    }
    s->data[s->top++] = value; // 压入值+栈顶上移
    return 1;
}

// 栈弹出元素（出栈）
// 参数：s-栈指针，out-输出参数（保存弹出值）
// 返回：1成功，0失败（栈空）
int stack_pop(Stack *s, int *out) {
    if (s->top == 0) { // 栈空
        return 0;
    }
    *out = s->data[--s->top]; // 栈顶下移+取出值
    return 1;
}

// 查看栈顶元素（不出栈）
// 参数：s-栈指针（只读），out-输出参数（保存栈顶值）
// 返回：1成功，0失败（栈空）
int stack_peek(const Stack *s, int *out) {
    if (s->top == 0) { // 栈空
        return 0;
    }
    *out = s->data[s->top - 1]; // 取栈顶值（不移动指针）
    return 1;
}

// -----------------------------
// 队列（循环数组实现）- 先进先出（FIFO）
// -----------------------------
// 循环队列结构体：数组+头指针+尾指针+有效元素数
typedef struct {
    int data[16]; // 队列空间，容量16
    int head;     // 队头指针（指向队头元素）
    int tail;     // 队尾指针（指向队尾下一个位置）
    int size;     // 有效元素数（避免头/尾指针重合时判断空/满）
} Queue;

// 初始化队列（空队列）
void queue_init(Queue *q) {
    q->head = 0;
    q->tail = 0;
    q->size = 0;
}

// 入队（尾插）
// 参数：q-队列指针，value-入队值
// 返回：1成功，0失败（队列满）
int queue_enqueue(Queue *q, int value) {
    if (q->size >= 16) { // 队列满
        return 0;
    }
    q->data[q->tail] = value;                // 尾指针位置赋值
    q->tail = (q->tail + 1) % 16;           // 尾指针循环后移
    q->size++;                               // 有效元素数+1
    return 1;
}

// 出队（头删）
// 参数：q-队列指针，out-输出参数（保存出队值）
// 返回：1成功，0失败（队列空）
int queue_dequeue(Queue *q, int *out) {
    if (q->size == 0) { // 队列空
        return 0;
    }
    *out = q->data[q->head];                // 取队头值
    q->head = (q->head + 1) % 16;           // 头指针循环后移
    q->size--;                               // 有效元素数-1
    return 1;
}

// 查看队头元素（不出队）
// 参数：q-队列指针（只读），out-输出参数（保存队头值）
// 返回：1成功，0失败（队列空）
int queue_front(const Queue *q, int *out) {
    if (q->size == 0) { // 队列空
        return 0;
    }
    *out = q->data[q->head]; // 取队头值
    return 1;
}

// -----------------------------
// 二叉树（普通二叉树）
// -----------------------------
// 二叉树节点结构体
typedef struct BNode {
    int value;          // 节点值
    struct BNode *left; // 左孩子指针
    struct BNode *right;// 右孩子指针
} BNode;

// 创建二叉树节点
// 参数：value-节点值
// 返回：新节点指针（失败返回NULL）
BNode *bnode_create(int value) {
    BNode *node = (BNode *)malloc(sizeof(BNode));
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// 二叉树前序遍历（根→左→右）
void btree_preorder(const BNode *root) {
    if (!root) return;          // 递归终止：空节点
    printf("%d ", root->value); // 访问根节点
    btree_preorder(root->left); // 递归左子树
    btree_preorder(root->right);// 递归右子树
}

// 二叉树中序遍历（左→根→右）
void btree_inorder(const BNode *root) {
    if (!root) return;          // 递归终止：空节点
    btree_inorder(root->left);  // 递归左子树
    printf("%d ", root->value); // 访问根节点
    btree_inorder(root->right); // 递归右子树
}

// 二叉树后序遍历（左→右→根）
void btree_postorder(const BNode *root) {
    if (!root) return;           // 递归终止：空节点
    btree_postorder(root->left); // 递归左子树
    btree_postorder(root->right);// 递归右子树
    printf("%d ", root->value);  // 访问根节点
}

// 计算二叉树高度（递归）
// 返回：树的高度（空树返回0）
int btree_height(const BNode *root) {
    if (!root) return 0; // 空树高度0
    int hl = btree_height(root->left);  // 左子树高度
    int hr = btree_height(root->right); // 右子树高度
    return (hl > hr ? hl : hr) + 1;     // 取最大值+1（当前层）
}

// 释放二叉树所有节点内存（后序遍历释放）
void btree_free(BNode *root) {
    if (!root) return;          // 递归终止：空节点
    btree_free(root->left);     // 释放左子树
    btree_free(root->right);    // 释放右子树
    free(root);                 // 释放当前节点
}

// -----------------------------
// 二叉搜索树（BST）- 左子树<根<右子树，无重复值
// -----------------------------
// BST插入节点（递归）
// 参数：root-根节点指针，value-插入值
// 返回：新的根节点指针
BNode *bst_insert(BNode *root, int value) {
    if (!root) return bnode_create(value); // 空树，创建新节点
    if (value < root->value) {             // 小于根，插入左子树
        root->left = bst_insert(root->left, value);
    } else if (value > root->value) {      // 大于根，插入右子树
        root->right = bst_insert(root->right, value);
    }
    // 等于根则不插入（无重复）
    return root;
}

// BST查找指定值（递归）
// 参数：root-根节点指针（只读），value-查找值
// 返回：找到返回节点指针，未找到返回NULL
const BNode *bst_search(const BNode *root, int value) {
    if (!root) return NULL; // 空树，未找到
    if (value == root->value) return root; // 找到
    if (value < root->value) return bst_search(root->left, value); // 查左子树
    return bst_search(root->right, value); // 查右子树
}

// 查找BST中最小值节点（用于删除）
// 返回：最小值节点指针
static BNode *bst_find_min(BNode *root) {
    while (root && root->left) { // 一直往左找
        root = root->left;
    }
    return root;
}

// BST删除节点（递归）
// 参数：root-根节点指针，value-删除值
// 返回：新的根节点指针
BNode *bst_remove(BNode *root, int value) {
    if (!root) return NULL; // 空树，返回NULL

    if (value < root->value) { // 小于根，删除左子树节点
        root->left = bst_remove(root->left, value);
    } else if (value > root->value) { // 大于根，删除右子树节点
        root->right = bst_remove(root->right, value);
    } else { // 找到要删除的节点
        // 情况1：叶子节点/只有右孩子
        if (!root->left) {
            BNode *right = root->right;
            free(root);
            return right;
        }
        // 情况2：只有左孩子
        if (!root->right) {
            BNode *left = root->left;
            free(root);
            return left;
        }
        // 情况3：有左右孩子 → 找右子树最小值节点替代
        BNode *min = bst_find_min(root->right);
        root->value = min->value; // 替换值
        root->right = bst_remove(root->right, min->value); // 删除最小值节点
    }
    return root;
}

// -----------------------------
// AVL树（平衡二叉搜索树）- 任意节点左右子树高度差≤1
// -----------------------------
// AVL节点结构体（比BST多高度字段）
typedef struct AVLNode {
    int value;          // 节点值
    int height;         // 节点高度（叶子节点为1）
    struct AVLNode *left;  // 左孩子
    struct AVLNode *right; // 右孩子
} AVLNode;

// 获取AVL节点高度（空节点返回0）
static int avl_height(const AVLNode *node) {
    return node ? node->height : 0;
}

// 取两个整数最大值
static int avl_max(int a, int b) {
    return a > b ? a : b;
}

// 创建AVL节点
static AVLNode *avl_create(int value) {
    AVLNode *node = (AVLNode *)malloc(sizeof(AVLNode));
    node->value = value;
    node->height = 1; // 新节点高度初始为1
    node->left = NULL;
    node->right = NULL;
    return node;
}

// AVL右旋（处理左左失衡）
// 参数：y-失衡节点
// 返回：旋转后的新根节点
static AVLNode *avl_rotate_right(AVLNode *y) {
    AVLNode *x = y->left;    // x是y的左孩子
    AVLNode *t2 = x->right;  // t2是x的右子树

    // 旋转操作
    x->right = y;
    y->left = t2;

    // 更新高度（先更y，再更x）
    y->height = avl_max(avl_height(y->left), avl_height(y->right)) + 1;
    x->height = avl_max(avl_height(x->left), avl_height(x->right)) + 1;

    return x; // x成为新根
}

// AVL左旋（处理右右失衡）
// 参数：x-失衡节点
// 返回：旋转后的新根节点
static AVLNode *avl_rotate_left(AVLNode *x) {
    AVLNode *y = x->right;   // y是x的右孩子
    AVLNode *t2 = y->left;   // t2是y的左子树

    // 旋转操作
    y->left = x;
    x->right = t2;

    // 更新高度（先更x，再更y）
    x->height = avl_max(avl_height(x->left), avl_height(x->right)) + 1;
    y->height = avl_max(avl_height(y->left), avl_height(y->right)) + 1;

    return y; // y成为新根
}

// 计算AVL节点的平衡因子（左高-右高）
static int avl_balance(const AVLNode *node) {
    return node ? avl_height(node->left) - avl_height(node->right) : 0;
}

// AVL插入节点（递归+平衡调整）
// 参数：node-当前节点指针，value-插入值
// 返回：平衡后的节点指针
AVLNode *avl_insert(AVLNode *node, int value) {
    // 1. 普通BST插入
    if (!node) return avl_create(value); // 空节点，创建新节点
    if (value < node->value) {           // 插入左子树
        node->left = avl_insert(node->left, value);
    } else if (value > node->value) {    // 插入右子树
        node->right = avl_insert(node->right, value);
    } else {
        return node; // 重复值，不插入
    }

    // 2. 更新当前节点高度
    node->height = 1 + avl_max(avl_height(node->left), avl_height(node->right));

    // 3. 计算平衡因子，判断是否失衡
    int balance = avl_balance(node);

    // 4. 平衡调整
    // 左左失衡 → 右旋
    if (balance > 1 && value < node->left->value) {
        return avl_rotate_right(node);
    }
    // 右右失衡 → 左旋
    if (balance < -1 && value > node->right->value) {
        return avl_rotate_left(node);
    }
    // 左右失衡 → 先左旋左孩子，再右旋当前节点
    if (balance > 1 && value > node->left->value) {
        node->left = avl_rotate_left(node->left);
        return avl_rotate_right(node);
    }
    // 右左失衡 → 先右旋右孩子，再左旋当前节点
    if (balance < -1 && value < node->right->value) {
        node->right = avl_rotate_right(node->right);
        return avl_rotate_left(node);
    }

    // 未失衡，返回原节点
    return node;
}

// AVL树中序遍历（有序输出）
void avl_inorder(const AVLNode *root) {
    if (!root) return;
    avl_inorder(root->left);
    printf("%d ", root->value);
    avl_inorder(root->right);
}

// 释放AVL树所有节点内存
void avl_free(AVLNode *root) {
    if (!root) return;
    avl_free(root->left);
    avl_free(root->right);
    free(root);
}

// -----------------------------
// 堆（最大堆）- 完全二叉树，父节点≥子节点，数组实现
// -----------------------------
// 最大堆结构体：数组+有效元素数
typedef struct {
    int data[32]; // 堆空间，容量32
    int size;     // 有效元素数
} Heap;

// 初始化堆（空堆）
void heap_init(Heap *h) {
    h->size = 0;
}

// 交换两个整数（堆排序辅助）
static void heap_swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

// 堆插入元素（上浮调整）
// 参数：h-堆指针，value-插入值
void heap_push(Heap *h, int value) {
    if (h->size >= 32) return; // 堆满，不插入
    int i = h->size++;         // 插入到最后位置，size+1
    h->data[i] = value;        // 赋值

    // 上浮调整：与父节点比较，大于则交换
    while (i > 0) {
        int parent = (i - 1) / 2; // 父节点索引
        if (h->data[parent] >= h->data[i]) break; // 满足最大堆，停止
        heap_swap(&h->data[parent], &h->data[i]); // 交换父子
        i = parent; // 继续向上调整
    }
}

// 堆弹出堆顶元素（最大值，下沉调整）
// 参数：h-堆指针，out-输出参数（保存堆顶值）
// 返回：1成功，0失败（堆空）
int heap_pop(Heap *h, int *out) {
    if (h->size == 0) return 0; // 堆空
    *out = h->data[0];          // 保存堆顶值
    h->data[0] = h->data[--h->size]; // 最后元素移到堆顶，size-1

    // 下沉调整：与左右孩子比较，小于则交换
    int i = 0;
    while (1) {
        int left = i * 2 + 1;  // 左孩子索引
        int right = i * 2 + 2; // 右孩子索引
        int largest = i;       // 最大元素索引（初始为当前）

        // 找左右孩子中更大的
        if (left < h->size && h->data[left] > h->data[largest]) largest = left;
        if (right < h->size && h->data[right] > h->data[largest]) largest = right;

        if (largest == i) break; // 已是最大，停止
        heap_swap(&h->data[i], &h->data[largest]); // 交换
        i = largest; // 继续向下调整
    }
    return 1;
}

// -----------------------------
// 图（邻接表实现）- 适用于稀疏图
// -----------------------------
// 图邻接表节点（边）
typedef struct GNode {
    int to;            // 边的终点
    struct GNode *next;// 下一条边
} GNode;

// 图结构体：节点数+邻接表数组
typedef struct {
    int n;             // 节点数
    GNode *adj[8];     // 邻接表（最多8个节点）
} Graph;

// 初始化图
// 参数：g-图指针，n-节点数
void graph_init(Graph *g, int n) {
    g->n = n;
    // 邻接表初始化为空
    for (int i = 0; i < 8; ++i) {
        g->adj[i] = NULL;
    }
}

// 添加有向边（内部函数）
// 参数：g-图指针，u-起点，v-终点
static void graph_add_edge_oneway(Graph *g, int u, int v) {
    // 创建边节点
    GNode *node = (GNode *)malloc(sizeof(GNode));
    node->to = v;
    node->next = g->adj[u]; // 头插法（顺序无关）
    g->adj[u] = node;       // 更新邻接表
}

// 添加无向边（u→v 和 v→u）
void graph_add_undirected(Graph *g, int u, int v) {
    graph_add_edge_oneway(g, u, v);
    graph_add_edge_oneway(g, v, u);
}

// 添加有向边（u→v）
void graph_add_directed(Graph *g, int u, int v) {
    graph_add_edge_oneway(g, u, v);
}

// 图的深度优先遍历（DFS，递归）
// 参数：g-图指针（只读），start-起始节点，visited-访问标记数组
void graph_dfs(const Graph *g, int start, int *visited) {
    visited[start] = 1; // 标记已访问
    printf("%d ", start); // 访问节点

    // 遍历邻接边
    for (GNode *cur = g->adj[start]; cur; cur = cur->next) {
        if (!visited[cur->to]) { // 未访问则递归
            graph_dfs(g, cur->to, visited);
        }
    }
}

// 图的广度优先遍历（BFS，队列实现）
// 参数：g-图指针（只读），start-起始节点
void graph_bfs(const Graph *g, int start) {
    int visited[8] = {0}; // 访问标记
    int queue[8];         // 队列（最多8个节点）
    int head = 0;         // 队头
    int tail = 0;         // 队尾

    visited[start] = 1;   // 标记起始节点
    queue[tail++] = start; // 入队

    while (head < tail) { // 队列非空
        int u = queue[head++]; // 出队
        printf("%d ", u);      // 访问节点

        // 遍历邻接边
        for (GNode *cur = g->adj[u]; cur; cur = cur->next) {
            if (!visited[cur->to]) { // 未访问
                visited[cur->to] = 1; // 标记
                queue[tail++] = cur->to; // 入队
            }
        }
    }
}

// 释放图的邻接表内存
void graph_free(Graph *g) {
    for (int i = 0; i < g->n; ++i) { // 遍历所有节点
        GNode *cur = g->adj[i];
        while (cur) { // 释放该节点的所有边
            GNode *next = cur->next;
            free(cur);
            cur = next;
        }
        g->adj[i] = NULL; // 置空
    }
}

// -----------------------------
// 哈希表（开放定址法-线性探测）- 解决哈希冲突
// -----------------------------
// 哈希表项结构体
typedef struct {
    int key;    // 键
    int value;  // 值
    int state;  // 状态：0-空，1-已用，2-已删除
} HashEntry;

// 哈希表结构体：表项数组+有效元素数
typedef struct {
    HashEntry table[23]; // 哈希表（大小23，质数减少冲突）
    int size;            // 有效元素数
} HashTable;

// 初始化哈希表（清空所有表项）
void hashtable_init(HashTable *ht) {
    memset(ht, 0, sizeof(HashTable)); // 内存置0，state=0，size=0
}

// 哈希函数（乘法哈希）
// 参数：key-键
// 返回：哈希索引（0~22）
static int hashtable_hash(int key) {
    // 2654435761是黄金比例数，减少冲突
    return (key * 2654435761u) % 23;
}

// 哈希表插入/更新键值对
// 参数：ht-哈希表指针，key-键，value-值
// 返回：1成功，0失败（表满）
int hashtable_put(HashTable *ht, int key, int value) {
    if (ht->size >= 17) { // 负载因子≈0.7（17/23），拒绝插入
        return 0;
    }
    int idx = hashtable_hash(key); // 计算初始索引

    // 线性探测：找空位置/已删除位置/相同键
    for (int i = 0; i < 23; ++i) {
        int pos = (idx + i) % 23; // 当前探测位置
        // 空/已删除位置，插入新值
        if (ht->table[pos].state != 1) {
            ht->table[pos].key = key;
            ht->table[pos].value = value;
            ht->table[pos].state = 1;
            ht->size++;
            return 1;
        }
        // 相同键，更新值
        if (ht->table[pos].state == 1 && ht->table[pos].key == key) {
            ht->table[pos].value = value;
            return 1;
        }
    }
    return 0; // 探测完所有位置，插入失败
}

// 哈希表查找指定键的值
// 参数：ht-哈希表指针（只读），key-键，out-输出参数（保存值）
// 返回：1成功，0失败（未找到）
int hashtable_get(const HashTable *ht, int key, int *out) {
    int idx = hashtable_hash(key); // 初始索引

    // 线性探测
    for (int i = 0; i < 23; ++i) {
        int pos = (idx + i) % 23;
        if (ht->table[pos].state == 0) {
            return 0; // 遇到空位置，停止探测（未找到）
        }
        // 找到有效键
        if (ht->table[pos].state == 1 && ht->table[pos].key == key) {
            *out = ht->table[pos].value;
            return 1;
        }
    }
    return 0; // 未找到
}

// 哈希表删除指定键（软删除：标记为已删除）
// 参数：ht-哈希表指针，key-键
// 返回：1成功，0失败（未找到）
int hashtable_remove(HashTable *ht, int key) {
    int idx = hashtable_hash(key); // 初始索引

    // 线性探测
    for (int i = 0; i < 23; ++i) {
        int pos = (idx + i) % 23;
        if (ht->table[pos].state == 0) {
            return 0; // 遇到空位置，停止探测
        }
        // 找到有效键，标记为已删除
        if (ht->table[pos].state == 1 && ht->table[pos].key == key) {
            ht->table[pos].state = 2;
            ht->size--;
            return 1;
        }
    }
    return 0; // 未找到
}

// -----------------------------
// 主函数：演示所有数据结构的基本用法
// -----------------------------
int main(void) {
    // 1. 顺序表演示
    SeqList sl;
    seqlist_init(&sl);
    seqlist_insert(&sl, 0, 10);   // 插入10到位置0
    seqlist_insert(&sl, 1, 20);   // 插入20到位置1
    seqlist_insert(&sl, 1, 15);   // 插入15到位置1
    seqlist_print(&sl);           // 打印：10 15 20
    seqlist_remove(&sl, 1);       // 删除位置1的15
    printf("SeqList find 20 at %d\n", seqlist_find(&sl, 20)); // 查找20，返回1

    // 2. 动态数组演示
    DynArray da;
    dynarray_init(&da, 4);        // 初始容量4
    dynarray_push_back(&da, 1);   // 尾插1
    dynarray_push_back(&da, 2);   // 尾插2
    dynarray_insert(&da, 1, 99);  // 位置1插入99
    dynarray_remove(&da, 0);      // 删除位置0的1
    dynarray_print(&da);          // 打印：99 2
    dynarray_free(&da);           // 释放内存

    // 3. 双向链表演示
    DList dl;
    dlist_init(&dl);
    dlist_push_back(&dl, 3);      // 尾插3
    dlist_push_front(&dl, 2);     // 头插2
    dlist_push_back(&dl, 4);      // 尾插4
    dlist_print_forward(&dl);     // 正向打印：2 3 4
    dlist_print_backward(&dl);    // 反向打印：4 3 2
    dlist_remove_value(&dl, 3);   // 删除值为3的节点
    dlist_print_forward(&dl);     // 打印：2 4
    dlist_clear(&dl);             // 清空链表

    // 4. 循环链表演示
    CList cl;
    clist_init(&cl);
    clist_push_back(&cl, 7);      // 尾插7
    clist_push_back(&cl, 8);      // 尾插8
    clist_push_back(&cl, 9);      // 尾插9
    clist_print_once(&cl);        // 打印：7 8 9
    clist_remove_value(&cl, 8);   // 删除值为8的节点
    clist_print_once(&cl);        // 打印：7 9
    clist_clear(&cl);             // 清空链表

    // 5. 栈演示
    Stack st;
    stack_init(&st);
    stack_push(&st, 11);          // 压入11
    stack_push(&st, 22);          // 压入22
    int top;
    if (stack_peek(&st, &top)) {  // 查看栈顶
        printf("Stack top: %d\n", top); // 输出22
    }
    while (stack_pop(&st, &top)) { // 弹出所有元素
        printf("Stack pop: %d\n", top); // 先22，后11
    }

    // 6. 队列演示
    Queue q;
    queue_init(&q);
    queue_enqueue(&q, 5);         // 入队5
    queue_enqueue(&q, 6);         // 入队6
    int front;
    if (queue_front(&q, &front)) { // 查看队头
        printf("Queue front: %d\n", front); // 输出5
    }
    while (queue_dequeue(&q, &front)) { // 出队所有元素
        printf("Queue dequeue: %d\n", front); // 先5，后6
    }

    // 7. 二叉树演示
    BNode *bt = bnode_create(1);  // 根节点1
    bt->left = bnode_create(2);   // 左孩子2
    bt->right = bnode_create(3);  // 右孩子3
    bt->left->left = bnode_create(4); // 2的左孩子4
    bt->left->right = bnode_create(5); // 2的右孩子5
    printf("BT preorder: ");
    btree_preorder(bt);           // 前序：1 2 4 5 3
    printf("\nBT inorder: ");
    btree_inorder(bt);            // 中序：4 2 5 1 3
    printf("\nBT postorder: ");
    btree_postorder(bt);          // 后序：4 5 2 3 1
    printf("\nBT height: %d\n", btree_height(bt)); // 高度3
    btree_free(bt);               // 释放二叉树

    // 8. 二叉搜索树演示
    BNode *bst = NULL;
    bst = bst_insert(bst, 8);     // 插入8（根）
    bst = bst_insert(bst, 3);     // 插入3（左）
    bst = bst_insert(bst, 10);    // 插入10（右）
    bst = bst_insert(bst, 1);     // 插入1（3的左）
    bst = bst_insert(bst, 6);     // 插入6（3的右）
    printf("BST inorder: ");
    btree_inorder(bst);           // 中序：1 3 6 8 10
    printf("\nBST search 6: %s\n", bst_search(bst, 6) ? "found" : "not found"); // found
    bst = bst_remove(bst, 3);     // 删除3
    printf("BST inorder after remove: ");
    btree_inorder(bst);           // 中序：1 6 8 10
    printf("\n");
    btree_free(bst);              // 释放BST

    // 9. AVL树演示
    AVLNode *avl = NULL;
    int avl_values[] = {10, 20, 30, 40, 50, 25}; // 插入会触发旋转
    for (int i = 0; i < 6; ++i) {
        avl = avl_insert(avl, avl_values[i]);
    }
    printf("AVL inorder: ");
    avl_inorder(avl);             // 中序：10 20 25 30 40 50
    printf("\n");
    avl_free(avl);                // 释放AVL

    // 10. 堆演示
    Heap heap;
    heap_init(&heap);
    heap_push(&heap, 4);          // 插入4
    heap_push(&heap, 15);         // 插入15（堆顶）
    heap_push(&heap, 7);          // 插入7
    int max;
    if (heap_pop(&heap, &max)) {  // 弹出堆顶
        printf("Heap pop: %d\n", max); // 输出15
    }

    // 11. 无向图演示
    Graph ug;
    graph_init(&ug, 5);           // 5个节点
    graph_add_undirected(&ug, 0, 1); // 边0-1
    graph_add_undirected(&ug, 0, 2); // 边0-2
    graph_add_undirected(&ug, 1, 3); // 边1-3
    graph_add_undirected(&ug, 2, 4); // 边2-4
    int visited_ug[8] = {0};
    printf("UG DFS: ");
    graph_dfs(&ug, 0, visited_ug); // DFS：0 2 4 1 3
    printf("\nUG BFS: ");
    graph_bfs(&ug, 0);            // BFS：0 1 2 3 4
    printf("\n");
    graph_free(&ug);              // 释放图

    // 12. 有向图演示
    Graph dg;
    graph_init(&dg, 5);           // 5个节点
    graph_add_directed(&dg, 0, 1); // 边0→1
    graph_add_directed(&dg, 0, 2); // 边0→2
    graph_add_directed(&dg, 1, 3); // 边1→3
    graph_add_directed(&dg, 2, 3); // 边2→3
    graph_add_directed(&dg, 3, 4); // 边3→4
    int visited_dg[8] = {0};
    printf("DG DFS: ");
    graph_dfs(&dg, 0, visited_dg); // DFS：0 2 3 4 1
    printf("\nDG BFS: ");
    graph_bfs(&dg, 0);            // BFS：0 1 2 3 4
    printf("\n");
    graph_free(&dg);              // 释放图

    // 13. 哈希表演示
    HashTable ht;
    hashtable_init(&ht);
    hashtable_put(&ht, 42, 100);  // 插入(42,100)
    hashtable_put(&ht, 7, 200);   // 插入(7,200)
    int value;
    if (hashtable_get(&ht, 42, &value)) { // 查找42
        printf("Hash get 42: %d\n", value); // 输出100
    }
    hashtable_remove(&ht, 7);     // 删除7
    printf("Hash get 7: %s\n", hashtable_get(&ht, 7, &value) ? "found" : "not found"); // not found

    return 0;
}