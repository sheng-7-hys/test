#include<stdio.h>
#include<stdlib.h>
int main(){
    int n;
    scanf("%d",&n);
    int *p1=(int*)malloc(sizeof(int)*n);
    for(int i=0;i<n;i++){
        scanf("%d",p1+i);
    }

    // 处理最小n=1的边界情况
    if(n == 1){
        printf("%d", *p1);
        free(p1);
        return 0;
    }

    int maxcount=0;
    int count=0;
    int *p2=p1;

    for(int i=1;i < n;i++){
        if(*(p1+i) > *(p1+i-1)){  // 严格递增，相等不计数
            count++;
            // 仅当更长时更新，保证输出第一组最长段
            if(count > maxcount){
                maxcount=count;
                p2=p1+i;
            }
        }
        else{
            count=0;
        }
    }

    // 无多余空格的输出逻辑
    printf("%d", *(p2 - maxcount));
    for(int i=1; i<=maxcount; i++){
        printf(" %d", *(p2 - maxcount + i));
    }

    free(p1);  // 释放内存，避免内存泄漏
    return 0;
}