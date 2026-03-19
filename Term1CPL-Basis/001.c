#include<stdio.h>
int max(int a,int b){
    return a>b?a:b;
}
int main(){
    int(*p)(int,int);
    p=max;
    int x,y;
    scanf("%d %d",&x,&y);
    printf("max=%d\n",p(x,y));
    return 0;
}