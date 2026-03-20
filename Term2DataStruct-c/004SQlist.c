#include<stdio.h>
#include<stdlib.h>
#define ListInitSize 100
#define ListIncreasement 10

typedef struct{
    int *elem;
    int length;
    int listsize;
}sqlist; //define the struct

void InitList(sqlist *L){
    L->elem=(int *)malloc(ListInitSize*sizeof(int));
    if(!L->elem)exit(-1);
    L->length=0;
    L->listsize=ListInitSize;
    exit (0);
}//init the sqlist