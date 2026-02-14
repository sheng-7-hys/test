/*
 * 008.c
 * 演示动态单链表的创建、插入、删除、修改、查找、遍历、释放
 * 编译: gcc -g 008.c -o 008.exe
 * 运行: ./008.exe
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	int data;
	struct Node *next;//指向下一个结点的指针
} Node;//定义结点结构体

/* 创建新结点 */
Node* create_node(int val) {
	Node *p = (Node*)malloc(sizeof(Node));//动态分配内存
	if (!p) {
		fprintf(stderr, "内存分配失败\n");
		exit(EXIT_ FAILURE);//分配失败则退出程序
	}
	p->data = val;
	p->next = NULL;//新结点的下一个结点指针初始化为 NULL
	return p;
}

/* 在头部插入 */
void insert_head(Node **head, int val) {
	Node *p = create_node(val);
	p->next = *head;
	*head = p;
}

/* 在尾部插入 */
void insert_tail(Node **head, int val) {
	Node *p = create_node(val);
	if (*head == NULL) {
		*head = p;
		return;
	}
	Node *cur = *head;
	while (cur->next) cur = cur->next;//到达原链表末尾
	cur->next = p;
}

/* 在值为 target 的结点后插入新结点（只插入到第一个匹配处）
 * 返回 1 表示插入成功，0 表示未找到 target */
int insert_after(Node *head, int target, int val) {
	Node *cur = head;
	while (cur) {
		if (cur->data == target) {
			Node *p = create_node(val);
			p->next = cur->next; //先连接新结点的下一个结点
			cur->next = p;  //再连接当前结点到新结点
			return 1;
		}
		cur = cur->next;//实现遍历
	}
	return 0;
}

/* 删除值为 val 的第一个结点
 * 返回 1 表示删除成功，0 表示未找到 */
int delete_by_value(Node **head, int val) {
	if (*head == NULL) return 0;
	Node *cur = *head;
	Node *prev = NULL;
	while (cur) {
		if (cur->data == val) {
			if (prev == NULL) { /* 删除头结点 */
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

/* 将第一个匹配 old_val 的结点的值改为 new_val
 * 返回 1 表示修改成功，0 表示未找到 */
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

/* 查找值为 val 的结点，找到返回指针，否则返回 NULL */
Node* search(Node *head, int val) {
	Node *cur = head;
	while (cur) {
		if (cur->data == val) return cur;
		cur = cur->next;
	}
	return NULL;
}

/* 遍历并打印链表 */
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

/* 释放整个链表 */
void free_list(Node **head) {
	Node *cur = *head;
	while (cur) {
		Node *tmp = cur;
		cur = cur->next;
		free(tmp);
	}
	*head = NULL;
}

int main(void) {
	Node *head = NULL; /* 初始空链表 */

	/* 插入尾部元素 */
	insert_tail(&head, 10);
	insert_tail(&head, 20);
	insert_tail(&head, 30);
	printf("初始: "); traverse(head);

	/* 在头部插入 */
	insert_head(&head, 5);
	printf("插入头部 5: "); traverse(head);

	/* 在值为 20 的结点后插入 25 */
	if (insert_after(head, 20, 25)) {
		printf("在 20 后插入 25: "); traverse(head);
	} else {
		printf("未找到 20，插入失败\n");
	}

	/* 修改值 25 为 26 */
	if (modify_value(head, 25, 26)) {
		printf("将 25 修改为 26: "); traverse(head);
	}

	/* 删除值 10 的结点 */
	if (delete_by_value(&head, 10)) {
		printf("删除值 10 后: "); traverse(head);
	}

	/* 查找元素 30 */
	Node *found = search(head, 30);
	if (found) printf("查找 30: 找到，节点地址=%p，值=%d\n", (void*)found, found->data);
	else printf("查找 30: 未找到\n");

	/* 释放链表 */
	free_list(&head);
	printf("释放后，head=%p\n", (void*)head);

	return 0;
}
