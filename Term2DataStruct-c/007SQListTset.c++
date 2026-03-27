#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
typedef struct{
    int *elem; //存储空间基址
    int length;
    int listsize;
}sqlist;
bool init(sqlist &L){
    L.elem=(int *)malloc(20*sizeof(int));
    if(!L.elem)exit(1);
    L.length=0;
    L.listsize=20;
    return true;
}

bool insert(sqlist &L,int i,int e){ //i is location and e is data
    //verify the legality of "i"
    if(i<1||i>L.length+1)exit(1);
    if(L.length>=L.listsize){ //if need to expand the space?
        int* newbase = (int *)realloc(L.elem,(L.listsize+5)*sizeof(int));
        if(!newbase)exit(1);
        L.elem=newbase;
        L.listsize+=5;
    }
    int*q=&(L.elem[i-1]);
    for(int *p=&(L.elem[L.length-1]);p>=q;--p) *(p+1)=*p;
    *q=e;
    ++L.length;
    return true;

}

int main(){
    sqlist L1,L2;
    init(L1);
    init(L2);
    printf("The elem positions are %p and %p",L1.elem,L2.elem);
    insert(L1,1,7);
    printf("\n%d\n",L1.elem[0]);
    insert(L1,2,7);
    insert(L1,3,7);
    insert(L1,2,8);
    for(int i=0;i<L1.length;i++){
        printf("%d",L1.elem[i]);
    }
    insert(L1,7,7);
    return 0;
}