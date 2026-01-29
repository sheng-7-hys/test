/*
 * 007.c
 * 演示动态单链表（含交互菜单）、双向链表、循环单链表的创建、插入、删除、修改、查找、遍历、释放
 * 编译: gcc -g 007.c -o 007.exe
 * 运行: ./007.exe
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/* ------------------------- 工具函数 ------------------------- */
static void clear_stdin(void) {
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF) {}
}

/* ------------------------- 单链表 ------------------------- */
typedef struct Node {
	int data;
	struct Node *next;
} Node;

Node* create_node(int val) {
	Node *p = (Node*)malloc(sizeof(Node));
	if (!p) {
		fprintf(stderr, "内存分配失败\n");
		exit(EXIT_FAILURE);
	}
	p->data = val;
	p->next = NULL;
	return p;
}

void insert_head(Node **head, int val) {
	Node *p = create_node(val);
	p->next = *head;
	*head = p;
}

void insert_tail(Node **head, int val) {
	Node *p = create_node(val);
	if (*head == NULL) {
		*head = p;
		return;
	}
	Node *cur = *head;
	while (cur->next) cur = cur->next;
	cur->next = p;
}

int insert_after(Node *head, int target, int val) {
	Node *cur = head;
	while (cur) {
		if (cur->data == target) {
			Node *p = create_node(val);
			p->next = cur->next;
			cur->next = p;
			return 1;
		}
		cur = cur->next;
	}
	return 0;
}

int delete_by_value(Node **head, int val) {
	if (*head == NULL) return 0;
	Node *cur = *head;
	Node *prev = NULL;
	while (cur) {
		if (cur->data == val) {
			if (prev == NULL) {
				*head = cur->next;
			} else {
				prev->next = cur->next;
			}
			free(cur);
			return 1;
		}
		prev = cur;
		cur = cur->next;
	}
	return 0;
}

int modify_value(Node *head, int old_val, int new_val) {
	Node *cur = head;
	while (cur) {
		if (cur->data == old_val) {
			cur->data = new_val;
			return 1;
		}
		cur = cur->next;
	}
	return 0;
}

Node* search(Node *head, int val) {
	Node *cur = head;
	while (cur) {
		if (cur->data == val) return cur;
		cur = cur->next;
	}
	return NULL;
}

void traverse(Node *head) {
	Node *cur = head;
	printf("链表: ");
	while (cur) {
		printf("%d", cur->data);
		if (cur->next) printf(" -> ");
		cur = cur->next;
	}
	printf("\n");
}

void free_list(Node **head) {
	Node *cur = *head;
	while (cur) {
		Node *tmp = cur;
		cur = cur->next;
		free(tmp);
	}
	*head = NULL;
}

static void scripted_singly_demo(void) {
	Node *head = NULL;
	insert_tail(&head, 10);
	insert_tail(&head, 20);
	insert_tail(&head, 30);
	printf("初始: "); traverse(head);

	insert_head(&head, 5);
	printf("插入头部 5: "); traverse(head);

	if (insert_after(head, 20, 25)) {
		printf("在 20 后插入 25: "); traverse(head);
	} else {
		printf("未找到 20，插入失败\n");
	}

	if (modify_value(head, 25, 26)) {
		printf("将 25 修改为 26: "); traverse(head);
	}

	if (delete_by_value(&head, 10)) {
		printf("删除值 10 后: "); traverse(head);
	}

	Node *found = search(head, 30);
	if (found) printf("查找 30: 找到，节点地址=%p，值=%d\n", (void*)found, found->data);
	else printf("查找 30: 未找到\n");

	free_list(&head);
	printf("释放后，head=%p\n", (void*)head);
}

static void menu_singly(void) {
	Node *head = NULL;
	int choice;
	int val, target;
	printf("\n=== 单链表交互菜单 ===\n");
	while (1) {
		printf("1. 头插  2. 尾插  3. 在指定值后插入  4. 删除值  5. 修改值  6. 查找值  7. 遍历  8. 退出\n");
		printf("请选择: ");
		if (scanf("%d", &choice) != 1) {
			printf("输入无效，重试\n");
			clear_stdin();
			continue;
		}
		clear_stdin();
		switch (choice) {
		case 1:
			printf("输入要头插的值: ");
			scanf("%d", &val); clear_stdin();
			insert_head(&head, val); traverse(head);
			break;
		case 2:
			printf("输入要尾插的值: ");
			scanf("%d", &val); clear_stdin();
			insert_tail(&head, val); traverse(head);
			break;
		case 3:
			printf("目标值: "); scanf("%d", &target);
			printf("新值: "); scanf("%d", &val); clear_stdin();
			if (insert_after(head, target, val)) traverse(head);
			else printf("未找到目标值\n");
			break;
		case 4:
			printf("要删除的值: "); scanf("%d", &val); clear_stdin();
			if (delete_by_value(&head, val)) traverse(head);
			else printf("未找到该值\n");
			break;
		case 5:
			printf("旧值: "); scanf("%d", &target);
			printf("新值: "); scanf("%d", &val); clear_stdin();
			if (modify_value(head, target, val)) traverse(head);
			else printf("未找到该值\n");
			break;
		case 6: {
			printf("要查找的值: "); scanf("%d", &val); clear_stdin();
			Node *found = search(head, val);
			if (found) printf("找到，节点地址=%p，值=%d\n", (void*)found, found->data);
			else printf("未找到\n");
			break; }
		case 7:
			traverse(head);
			break;
		case 8:
			free_list(&head);
			printf("退出菜单，链表已释放\n\n");
			return;
		default:
			printf("无效选择\n");
		}
	}
}

/* ------------------------- 双向链表 ------------------------- */
typedef struct DNode {
	int data;
	struct DNode *prev;
	struct DNode *next;
} DNode;

static DNode* d_create_node(int val) {
	DNode *p = (DNode*)malloc(sizeof(DNode));
	if (!p) { fprintf(stderr, "内存分配失败\n"); exit(EXIT_FAILURE); }
	p->data = val; p->prev = p->next = NULL; return p;
}

static void d_insert_tail(DNode **head, int val) {
	DNode *p = d_create_node(val);
	if (*head == NULL) { *head = p; return; }
	DNode *cur = *head;
	while (cur->next) cur = cur->next;
	cur->next = p; p->prev = cur;
}

static void d_insert_head(DNode **head, int val) {
	DNode *p = d_create_node(val);
	p->next = *head;
	if (*head) (*head)->prev = p;
	*head = p;
}

static int d_delete(DNode **head, int val) {
	DNode *cur = *head;
	while (cur) {
		if (cur->data == val) {
			if (cur->prev) cur->prev->next = cur->next;
			else *head = cur->next;
			if (cur->next) cur->next->prev = cur->prev;
			free(cur);
			return 1;
		}
		cur = cur->next;
	}
	return 0;
}

static void d_traverse(DNode *head) {
	printf("正向: ");
	DNode *cur = head, *tail = NULL;
	while (cur) {
		tail = cur;
		printf("%d", cur->data);
		if (cur->next) printf(" <-> ");
		cur = cur->next;
	}
	printf("\n");
	printf("反向: ");
	cur = tail;
	while (cur) {
		printf("%d", cur->data);
		if (cur->prev) printf(" <-> ");
		cur = cur->prev;
	}
	printf("\n");
}

static void d_free(DNode **head) {
	DNode *cur = *head;
	while (cur) {
		DNode *tmp = cur;
		cur = cur->next;
		free(tmp);
	}
	*head = NULL;
}

static void demo_doubly(void) {
	DNode *head = NULL;
	d_insert_tail(&head, 1);
	d_insert_tail(&head, 2);
	d_insert_tail(&head, 3);
	d_insert_head(&head, 0);
	printf("双向链表初始: \n"); d_traverse(head);
	if (d_delete(&head, 2)) {
		printf("删除 2 后: \n"); d_traverse(head);
	}
	d_free(&head);
	printf("双向链表已释放\n\n");
}

/* ------------------------- 循环单链表 ------------------------- */
typedef struct CNode {
	int data;
	struct CNode *next;
} CNode;

static CNode* c_create_node(int val) {
	CNode *p = (CNode*)malloc(sizeof(CNode));
	if (!p) { fprintf(stderr, "内存分配失败\n"); exit(EXIT_FAILURE); }
	p->data = val; p->next = NULL; return p;
}

/* tail 指向最后一个结点，tail->next 为头结点 */
static void c_insert_tail(CNode **tail, int val) {
	CNode *p = c_create_node(val);
	if (*tail == NULL) {
		*tail = p;
		p->next = p; /* 自环 */
		return;
	}
	p->next = (*tail)->next;
	(*tail)->next = p;
	*tail = p;
}

static int c_delete(CNode **tail, int val) {
	if (*tail == NULL) return 0;
	CNode *cur = (*tail)->next; /* 头 */
	CNode *prev = *tail;
	do {
		if (cur->data == val) {
			if (cur == prev) {
				/* 只有一个结点 */
				free(cur);
				*tail = NULL;
			} else {
				prev->next = cur->next;
				if (cur == *tail) *tail = prev;
				free(cur);
			}
			return 1;
		}
		prev = cur;
		cur = cur->next;
	} while (cur != (*tail)->next);
	return 0;
}

static void c_traverse(CNode *tail) {
	if (tail == NULL) {
		printf("循环链表: 空\n");
		return;
	}
	CNode *cur = tail->next; /* 头 */
	printf("循环链表: ");
	do {
		printf("%d", cur->data);
		cur = cur->next;
		if (cur != tail->next) printf(" -> ");
	} while (cur != tail->next);
	printf(" (回到头)\n");
}

static void c_free(CNode **tail) {
	if (*tail == NULL) return;
	CNode *start = (*tail)->next;
	CNode *cur = start;
	do {
		CNode *tmp = cur;
		cur = cur->next;
		free(tmp);
	} while (cur != start);
	*tail = NULL;
}

static void demo_circular(void) {
	CNode *tail = NULL;
	c_insert_tail(&tail, 7);
	c_insert_tail(&tail, 8);
	c_insert_tail(&tail, 9);
	c_traverse(tail);
	if (c_delete(&tail, 8)) {
		printf("删除 8 后: "); c_traverse(tail);
	}
	c_free(&tail);
	printf("循环链表已释放\n\n");
}

/* ------------------------- 主程序 ------------------------- */
int main(void) {
	printf("=== 单链表脚本演示 ===\n");
	scripted_singly_demo();

	printf("是否进入单链表交互菜单? (y/n): ");
	int ch = getchar();
	if (ch == '\n') ch = 'n';
	if (tolower(ch) == 'y') {
		menu_singly();
	} else {
		printf("跳过交互菜单\n\n");
	}

	printf("=== 双向链表演示 ===\n");
	demo_doubly();

	printf("=== 循环单链表演示 ===\n");
	demo_circular();

	return 0;
}

