#include<stdio.h>
int main(){
    char *name[5]={"Alice","Bob","Charlie","David","Eve"};
    char **p;
    int i;
    for(i=0;i<5;i++){
        p=name+i;
        printf ("name[%d] = %p, address=%d\n",i,*p,p);
        printf ("name[%d] = %s, address=%p\n",i,*p,name+i);
    }
    return 0;
}