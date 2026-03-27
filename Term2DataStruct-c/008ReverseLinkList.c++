#include <stdio.h>
#include <stdlib.h>

typedef int ElementType;
typedef struct Node *PtrToNode;
struct Node {
    ElementType Data;
    PtrToNode   Next;
};
typedef PtrToNode List;

List Read(); /* 细节在此不表 */
void Print( List L ); /* 细节在此不表 */

List Reverse( List L );

int main()
{
    List L1, L2;
    L1 = Read();
    L2 = Reverse(L1);
    Print(L1);
    Print(L2);
    return 0;
}

/* 你的代码将被嵌在这里 */
List Read(){
    List L = (List)malloc(sizeof(struct Node));
    L->Next = NULL;
    PtrToNode tail = L;
    int n, data;
    scanf("%d", &n);
    for(int i = 0; i < n; i++){
        scanf("%d", &data);
        PtrToNode newNode = (PtrToNode)malloc(sizeof(struct Node));
        newNode->Data = data;
        newNode->Next = NULL;
        tail->Next = newNode;
        tail = newNode;
    }
    return L;
}
void Print( List L ){
    PtrToNode current = L->Next;
    while(current != NULL){
        printf("%d ", current->Data);
        current = current->Next;
    }
    printf("\n");
}
List Reverse( List L ) {//通过指针方向的反转来实现链表的反转
    // 定义三个指针：前驱、当前、后继
    PtrToNode prev = NULL;  // 前驱节点初始为空
    PtrToNode curr = L;     // 当前节点从链表头开始
    PtrToNode next = NULL;  // 后继节点初始为空

    // 遍历整个链表，逐个反转指针
    while (curr != NULL) {
        next = curr->Next;  // 保存当前节点的下一个节点（防止断链）
        curr->Next = prev;  // 反转当前节点的指针，指向前驱节点
        prev = curr;        // 前驱节点后移
        curr = next;        // 当前节点后移
    }
    // 循环结束后，prev指向反转后的新头节点
    L = prev;
    return L;
}