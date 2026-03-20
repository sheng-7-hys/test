#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

#define LISTINCREMENT 5//线性表存储空间的分配增量
#define LIST_INIT_SIZE 10//线性表存储空间的初始分配量

typedef struct SqList{
    int *elem;//存储空间的基地址
    int length;//当前长度
    int listsize;//当前分配的存储容量，以sizeof(ElemType)为单位
}SqList;

int InsertOrderList(SqList &L,int x){
    // 1. 核心步骤：查找插入位置（找到第一个大于x的元素位置）
    int i = 0;
    while (i < L.length && L.elem[i] <= x) {
        i++;
    }

    // 2. 扩容判断：仅当表满时才扩容（修复冗余扩容问题）
    if (L.length >= L.listsize) {
        int *newbase = (int*)realloc(L.elem, sizeof(int)*(L.listsize + LISTINCREMENT));
        if (newbase == NULL) { // 检查内存分配是否成功，避免程序崩溃
            exit(1);
        }
        L.elem = newbase;     // 更新存储空间基地址
        L.listsize += LISTINCREMENT; // 更新总容量
    }

    // 3. 核心步骤：将插入位置后的元素向后移动一位（腾空间）
    for (int j = L.length; j > i; j--) {
        L.elem[j] = L.elem[j - 1];
    }

    // 4. 插入元素并更新表长度
    L.elem[i] = x;
    L.length++;

    return 0;
}

int main()
{
    SqList L;
    int i,x,m;
    L.elem=(int *)malloc(LIST_INIT_SIZE*sizeof(int));
    L.listsize=LIST_INIT_SIZE;
    L.length=0;
    m=scanf("%d",&L.length);
    for(i=0;i<L.length;i++)
        m=scanf("%d",&L.elem[i]);
    m=scanf("%d",&x);
    InsertOrderList(L,x);
    for(i=0;i<L.length;i++)
        printf("%d,",L.elem[i]);
    free(L.elem);
    return 0;
}