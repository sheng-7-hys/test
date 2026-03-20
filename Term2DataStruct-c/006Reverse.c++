#define LIST_INIT_SIZE 10
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
typedef struct{
    int *elem;//存储空间的基地址
    int length;//当前长度
    int listsize;//当前分配的存储容量，以sizeof(ElemType)为单位
}SqList;

void Reverse(SqList &L){
    for(int i=0;i<L.length/2;i++){
        int temp;
        temp=L.elem[i];
        L.elem[i]=L.elem[L.length-1-i];
        L.elem[L.length-1-i]=temp;
       
    }
     for(int i=0;i<L.length;i++){
            printf("%d,",L.elem[i]);
        }
}
int main()
{
  SqList L;
  int m;
  L.elem=(int*)malloc((LIST_INIT_SIZE)*sizeof(int));
  if(!L.elem) exit(-2);
  L.length=0;
  L.listsize=LIST_INIT_SIZE;
  m=scanf("%d",&L.length);
  for(int i=0;i<L.length;i++)  
     m=scanf("%d",&L.elem[i]);
  Reverse(L);
  return 0;
}